#include "scada_polling/polling_scheduler.h"

#include <algorithm>
#include <utility>

namespace dispatcher::polling
{
    PollingScheduler::PollingScheduler(
        const dispatcher::protocols::ProtocolDriverRegistry& driver_registry
    ) noexcept
        : driver_registry_(driver_registry)
    {
    }

    PollingGroupExecutionResult PollingScheduler::execute_group_once(
        const PollingGroup& group,
        const dispatcher::devices::DeviceConnection& connection,
        PollingSchedulerOptions options
    ) const
    {
        PollingGroupExecutionResult result;
        result.group_id = group.id;

        if (!group.is_enabled)
        {
            result.status = PollingExecutionStatus::Skipped;
            result.message = "Polling group is disabled.";
            return result;
        }

        if (!group.is_enabled_for_polling())
        {
            result.status = PollingExecutionStatus::Skipped;
            result.message = "Polling group is not ready for polling.";
            return result;
        }

        if (connection.protocol != group.protocol)
        {
            result.status = PollingExecutionStatus::Failed;
            result.message = "Polling group protocol does not match device connection protocol.";
            return result;
        }

        auto driver = driver_registry_.find_driver(group.protocol);

        if (!driver)
        {
            result.status = PollingExecutionStatus::Failed;
            result.message = "Protocol driver is not registered for polling group protocol.";
            return result;
        }

        if (options.check_connection_before_polling)
        {
            dispatcher::protocols::ProtocolConnectionCheckRequest check_request;
            check_request.device_id = group.device_id;
            check_request.connection = connection;
            check_request.timeout_ms = group.timeout_ms;
            check_request.correlation_id = options.correlation_id_prefix + ":" + group.id.str() + ":connection-check";

            const auto check_result = driver->check_connection(check_request);

            if (!check_result.is_success())
            {
                result.status = PollingExecutionStatus::Failed;
                result.message = check_result.has_message()
                    ? check_result.message
                    : "Protocol connection check failed.";

                return result;
            }
        }

        const auto tasks = enabled_tasks(group);

        if (tasks.empty())
        {
            result.status = PollingExecutionStatus::Skipped;
            result.message = "Polling group has no enabled tasks.";
            return result;
        }

        const auto capabilities = driver->capabilities();

        const auto can_use_batch =
            options.allow_batch_read &&
            group.can_use_batch_mode() &&
            capabilities.can_batch_read() &&
            tasks.size() <= group.max_batch_size &&
            tasks.size() <= capabilities.max_batch_size;

        if (can_use_batch)
        {
            const auto batch_request = make_batch_read_request(
                group,
                tasks,
                connection,
                options
            );

            const auto batch_result = driver->read_batch(batch_request);

            result.task_results.reserve(tasks.size());

            const auto returned_count = std::min(
                tasks.size(),
                batch_result.items.size()
            );

            for (std::size_t index = 0; index < returned_count; ++index)
            {
                result.task_results.push_back(
                    make_task_result(
                        *tasks[index],
                        batch_result.items[index]
                    )
                );
            }

            for (std::size_t index = returned_count; index < tasks.size(); ++index)
            {
                result.task_results.push_back(
                    make_failed_task_result(
                        *tasks[index],
                        "Batch read did not return result for polling task."
                    )
                );
            }

            result.message = batch_result.has_message()
                ? batch_result.message
                : "Polling group batch execution completed.";

            finalize_group_result(result);
            return result;
        }

        result.task_results.reserve(tasks.size());

        for (const auto* task : tasks)
        {
            const auto read_request = make_read_request(
                *task,
                connection,
                options
            );

            const auto read_result = driver->read(read_request);

            result.task_results.push_back(
                make_task_result(
                    *task,
                    read_result
                )
            );
        }

        result.message = "Polling group sequential execution completed.";
        finalize_group_result(result);

        return result;
    }

    std::vector<const PollingTask*> PollingScheduler::enabled_tasks(
        const PollingGroup& group
    )
    {
        std::vector<const PollingTask*> result;
        result.reserve(group.tasks.size());

        for (const auto& task : group.tasks)
        {
            if (task.is_enabled_for_polling())
            {
                result.push_back(&task);
            }
        }

        return result;
    }

    dispatcher::protocols::ProtocolReadRequest PollingScheduler::make_read_request(
        const PollingTask& task,
        const dispatcher::devices::DeviceConnection& connection,
        const PollingSchedulerOptions& options
    )
    {
        dispatcher::protocols::ProtocolReadRequest request;

        request.device_id = task.device_id;
        request.connection = connection;
        request.tag_id = task.tag_id;
        request.address = task.address;
        request.timeout_ms = task.timeout_ms;
        request.correlation_id = options.correlation_id_prefix + ":" + task.id.str();

        return request;
    }

    dispatcher::protocols::ProtocolBatchReadRequest PollingScheduler::make_batch_read_request(
        const PollingGroup& group,
        const std::vector<const PollingTask*>& tasks,
        const dispatcher::devices::DeviceConnection& connection,
        const PollingSchedulerOptions& options
    )
    {
        dispatcher::protocols::ProtocolBatchReadRequest request;

        request.device_id = group.device_id;
        request.connection = connection;
        request.timeout_ms = group.timeout_ms;
        request.correlation_id = options.correlation_id_prefix + ":" + group.id.str();

        request.items.reserve(tasks.size());

        for (const auto* task : tasks)
        {
            request.items.push_back(
                make_read_request(
                    *task,
                    connection,
                    options
                )
            );
        }

        return request;
    }

    PollingTaskExecutionResult PollingScheduler::make_task_result(
        const PollingTask& task,
        const dispatcher::protocols::ProtocolReadResult& read_result
    )
    {
        PollingTaskExecutionResult result;

        result.task_id = task.id;
        result.tag_id = task.tag_id;
        result.success = read_result.is_success();
        result.resulting_state = read_result.is_success()
            ? PollingTaskState::Succeeded
            : PollingTaskState::Failed;
        result.quality = read_result.quality;
        result.latency_ms = read_result.latency_ms;
        result.message = read_result.message;

        return result;
    }

    PollingTaskExecutionResult PollingScheduler::make_failed_task_result(
        const PollingTask& task,
        std::string message
    )
    {
        PollingTaskExecutionResult result;

        result.task_id = task.id;
        result.tag_id = task.tag_id;
        result.success = false;
        result.resulting_state = PollingTaskState::Failed;
        result.quality = dispatcher::tags::TagQuality::Bad;
        result.message = std::move(message);

        return result;
    }

    void PollingScheduler::finalize_group_result(
        PollingGroupExecutionResult& result
    ) noexcept
    {
        const auto total = result.task_count();

        if (total == 0)
        {
            result.status = PollingExecutionStatus::Skipped;
            return;
        }

        const auto success = result.success_count();

        if (success == total)
        {
            result.status = PollingExecutionStatus::Succeeded;
            return;
        }

        if (success == 0)
        {
            result.status = PollingExecutionStatus::Failed;
            return;
        }

        result.status = PollingExecutionStatus::PartialSuccess;
    }
}
