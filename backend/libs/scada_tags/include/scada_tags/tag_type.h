#pragma once

#include <string_view>

namespace dispatcher::tags
{
    enum class TagType
    {
        DeviceTag,
        ManualTag,
        CalculatedTag,
        SystemTag,
        VirtualTag,
        CommandTag,
        SimulationTag,
        ExternalTag
    };

    [[nodiscard]] std::string_view to_string(TagType type) noexcept;
}