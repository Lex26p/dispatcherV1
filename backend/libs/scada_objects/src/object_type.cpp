#include "scada_objects/object_type.h"

namespace dispatcher::objects
{
    std::string_view to_string(ObjectType type) noexcept
    {
        switch (type)
        {
        case ObjectType::Site:
            return "Site";
        case ObjectType::Building:
            return "Building";
        case ObjectType::Floor:
            return "Floor";
        case ObjectType::Room:
            return "Room";
        case ObjectType::Zone:
            return "Zone";
        case ObjectType::Cabinet:
            return "Cabinet";
        case ObjectType::Rack:
            return "Rack";
        case ObjectType::Line:
            return "Line";
        case ObjectType::EquipmentGroup:
            return "EquipmentGroup";
        case ObjectType::Custom:
            return "Custom";
        }

        return "Unknown";
    }
}