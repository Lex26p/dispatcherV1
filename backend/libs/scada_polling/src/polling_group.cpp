#include "scada_polling/polling_group.h"

namespace dispatcher::polling
{
    bool PollingGroup::has_valid_identity() const noexcept
    {
        return !id.empty() &&
            !device_id.empty() &&
            !code.empty() &&
            !name.empty();
    }

    bool PollingGroup::has_valid_timing() const noexcept
    {
        return interval_ms > 0 &&
            timeout_ms > 0;
    }

    bool PollingGroup::has_tasks() const noexcept
    {
        return !tasks.empty();
    }

    std::size_t PollingGroup::task_count() const noexcept
    {
        return tasks.size();
    }

    std::size_t PollingGroup::enabled_task_count() const noexcept
    {
        std::size_t result = 0;

        for (const auto& task : tasks)
        {
            if (task.is_enabled_for_polling())
            {
                ++result;
            }
        }

        return result;
    }

    bool PollingGroup::is_enabled_for_polling() const noexcept
    {
        return is_enabled &&
            has_valid_identity() &&
            has_valid_timing() &&
            enabled_task_count() > 0;
    }

    bool PollingGroup::can_use_batch_mode() const noexcept
    {
        return mode == PollingGroupMode::Batch &&
            max_batch_size > 1 &&
            enabled_task_count() > 1;
    }
}
