#pragma once

#include "scada_polling/polling_ids.h"
#include "scada_polling/polling_task_state.h"
#include "scada_tags/tag_ids.h"
#include "scada_tags/tag_quality.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::polling
{
    enum class PollingExecutionStatus
    {
        NotStarted,
        Skipped,
        Succeeded,
        Failed,
        PartialSuccess
    };

    struct PollingTaskExecutionResult
    {
        PollingTaskId task_id;
        dispatcher::tags::TagId tag_id;

        PollingTaskState resulting_state = PollingTaskState::Waiting;
        dispatcher::tags::TagQuality quality = dispatcher::tags::TagQuality::NotInitialized;

        bool success = false;
        std::uint32_t latency_ms = 0;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct PollingGroupExecutionResult
    {
        PollingGroupId group_id;

        PollingExecutionStatus status = PollingExecutionStatus::NotStarted;

        std::vector<PollingTaskExecutionResult> task_results;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_failures() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
        [[nodiscard]] std::size_t task_count() const noexcept;
        [[nodiscard]] std::size_t success_count() const noexcept;
        [[nodiscard]] std::size_t failure_count() const noexcept;
    };

    [[nodiscard]] std::string_view to_string(
        PollingExecutionStatus status
    ) noexcept;
}
