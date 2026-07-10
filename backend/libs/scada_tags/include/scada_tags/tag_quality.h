#pragma once

#include <string_view>

namespace dispatcher::tags
{
    enum class TagQuality
    {
        Good,
        Uncertain,
        Bad,
        CommunicationError,
        Timeout,
        DeviceError,
        ProtocolError,
        OutOfRange,
        Manual,
        Simulation,
        Maintenance,
        Stale,
        Disabled,
        NotInitialized
    };

    [[nodiscard]] std::string_view to_string(TagQuality quality) noexcept;
}
