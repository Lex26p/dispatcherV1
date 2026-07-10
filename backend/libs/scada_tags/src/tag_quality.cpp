#include "scada_tags/tag_quality.h"

namespace dispatcher::tags
{
    std::string_view to_string(TagQuality quality) noexcept
    {
        switch (quality)
        {
        case TagQuality::Good:
            return "Good";
        case TagQuality::Uncertain:
            return "Uncertain";
        case TagQuality::Bad:
            return "Bad";
        case TagQuality::CommunicationError:
            return "CommunicationError";
        case TagQuality::Timeout:
            return "Timeout";
        case TagQuality::DeviceError:
            return "DeviceError";
        case TagQuality::ProtocolError:
            return "ProtocolError";
        case TagQuality::OutOfRange:
            return "OutOfRange";
        case TagQuality::Manual:
            return "Manual";
        case TagQuality::Simulation:
            return "Simulation";
        case TagQuality::Maintenance:
            return "Maintenance";
        case TagQuality::Stale:
            return "Stale";
        case TagQuality::Disabled:
            return "Disabled";
        case TagQuality::NotInitialized:
            return "NotInitialized";
        }

        return "Unknown";
    }
}
