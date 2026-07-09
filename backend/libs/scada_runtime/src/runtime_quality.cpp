#include "scada_runtime/runtime_quality.h"

#include <variant>

namespace dispatcher::runtime
{
    bool is_good_runtime_quality(
        dispatcher::tags::TagQuality quality
    ) noexcept
    {
        switch (quality)
        {
        case dispatcher::tags::TagQuality::Good:
        case dispatcher::tags::TagQuality::Manual:
        case dispatcher::tags::TagQuality::Simulation:
            return true;

        case dispatcher::tags::TagQuality::Uncertain:
        case dispatcher::tags::TagQuality::Bad:
        case dispatcher::tags::TagQuality::CommunicationError:
        case dispatcher::tags::TagQuality::Timeout:
        case dispatcher::tags::TagQuality::DeviceError:
        case dispatcher::tags::TagQuality::ProtocolError:
        case dispatcher::tags::TagQuality::OutOfRange:
        case dispatcher::tags::TagQuality::Maintenance:
        case dispatcher::tags::TagQuality::Stale:
        case dispatcher::tags::TagQuality::Disabled:
        case dispatcher::tags::TagQuality::NotInitialized:
            return false;
        }

        return false;
    }

    bool is_bad_runtime_quality(
        dispatcher::tags::TagQuality quality
    ) noexcept
    {
        return !is_good_runtime_quality(quality);
    }

    bool can_update_last_good_value(
        dispatcher::tags::TagQuality quality,
        const dispatcher::tags::TagValuePayload& value
    ) noexcept
    {
        return is_good_runtime_quality(quality) &&
            !std::holds_alternative<std::monostate>(value);
    }
}