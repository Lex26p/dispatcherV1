#pragma once

#include "scada_devices/device_connection.h"
#include "scada_polling/polling_execution.h"
#include "scada_polling/polling_group.h"
#include "scada_polling/polling_task.h"
#include "scada_protocols/protocol_driver_registry.h"
#include "scada_protocols/protocol_requests.h"
#include "scada_protocols/protocol_results.h"

#include <string>
#include <vector>

namespace dispatcher::polling
{
    struct PollingSchedulerOptions
    {
        bool allow_batch_read = true;
        bool check_connection_before_polling = false;

        std::string correlation_id_prefix = "polling";
    };

    class PollingScheduler
    {
    public:
        explicit PollingScheduler(
            const dispatcher::protocols::ProtocolDriverRegistry& driver_registry
        ) noexcept;

        [[nodiscard]] PollingGroupExecutionResult execute_group_once(
            const PollingGroup& group,
            const dispatcher::devices::DeviceConnection& connection,
            PollingSchedulerOptions options = {}
        ) const;

    private:
        const dispatcher::protocols::ProtocolDriverRegistry& driver_registry_;

        [[nodiscard]] static std::vector<const PollingTask*> enabled_tasks(
            const PollingGroup& group
        );

        [[nodiscard]] static dispatcher::protocols::ProtocolReadRequest make_read_request(
            const PollingTask& task,
            const dispatcher::devices::DeviceConnection& connection,
            const PollingSchedulerOptions& options
        );

        [[nodiscard]] static dispatcher::protocols::ProtocolBatchReadRequest make_batch_read_request(
            const PollingGroup& group,
            const std::vector<const PollingTask*>& tasks,
            const dispatcher::devices::DeviceConnection& connection,
            const PollingSchedulerOptions& options
        );

        [[nodiscard]] static PollingTaskExecutionResult make_task_result(
            const PollingTask& task,
            const dispatcher::protocols::ProtocolReadResult& read_result
        );

        [[nodiscard]] static PollingTaskExecutionResult make_failed_task_result(
            const PollingTask& task,
            std::string message
        );

        static void finalize_group_result(
            PollingGroupExecutionResult& result
        ) noexcept;
    };
}