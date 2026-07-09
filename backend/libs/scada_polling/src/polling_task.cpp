#include "scada_polling/polling_task.h"

namespace dispatcher::polling
{
    bool PollingTask::has_valid_identity() const noexcept
    {
        return !id.empty() &&
            !group_id.empty() &&
            !device_id.empty() &&
            !tag_id.empty();
    }

    bool PollingTask::has_valid_timing() const noexcept
    {
        return interval_ms > 0 &&
            timeout_ms > 0;
    }

    bool PollingTask::has_address() const noexcept
    {
        return address.has_address() ||
            address.has_protocol_payload();
    }

    bool PollingTask::is_enabled_for_polling() const noexcept
    {
        return is_enabled &&
            state != PollingTaskState::Disabled &&
            has_valid_identity() &&
            has_valid_timing() &&
            has_address();
    }

    bool PollingTask::is_batch_compatible_with(
        const PollingTask& other
    ) const noexcept
    {
        return device_id == other.device_id &&
            protocol == other.protocol &&
            interval_ms == other.interval_ms &&
            timeout_ms == other.timeout_ms &&
            is_enabled &&
            other.is_enabled;
    }
}