#include "scada_polling/polling_execution.h"

namespace dispatcher::polling
{
    bool PollingTaskExecutionResult::is_success() const noexcept
    {
        return success;
    }

    bool PollingTaskExecutionResult::has_message() const noexcept
    {
        return !message.empty();
    }

    bool PollingGroupExecutionResult::is_success() const noexcept
    {
        return status == PollingExecutionStatus::Succeeded;
    }

    bool PollingGroupExecutionResult::has_failures() const noexcept
    {
        return failure_count() > 0;
    }

    bool PollingGroupExecutionResult::has_message() const noexcept
    {
        return !message.empty();
    }

    std::size_t PollingGroupExecutionResult::task_count() const noexcept
    {
        return task_results.size();
    }

    std::size_t PollingGroupExecutionResult::success_count() const noexcept
    {
        std::size_t result = 0;

        for (const auto& task_result : task_results)
        {
            if (task_result.is_success())
            {
                ++result;
            }
        }

        return result;
    }

    std::size_t PollingGroupExecutionResult::failure_count() const noexcept
    {
        std::size_t result = 0;

        for (const auto& task_result : task_results)
        {
            if (!task_result.is_success())
            {
                ++result;
            }
        }

        return result;
    }

    std::string_view to_string(
        PollingExecutionStatus status
    ) noexcept
    {
        switch (status)
        {
        case PollingExecutionStatus::NotStarted:
            return "NotStarted";
        case PollingExecutionStatus::Skipped:
            return "Skipped";
        case PollingExecutionStatus::Succeeded:
            return "Succeeded";
        case PollingExecutionStatus::Failed:
            return "Failed";
        case PollingExecutionStatus::PartialSuccess:
            return "PartialSuccess";
        }

        return "Unknown";
    }
}