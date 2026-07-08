#pragma once

#include <string_view>

namespace dispatcher::objects
{
    enum class ObjectType
    {
        Site,
        Building,
        Floor,
        Room,
        Zone,
        Cabinet,
        Rack,
        Line,
        EquipmentGroup,
        Custom
    };

    [[nodiscard]] std::string_view to_string(ObjectType type) noexcept;
}