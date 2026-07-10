#include "scada_objects/object_hierarchy_rules.h"

namespace dispatcher::objects
{
    namespace
    {
        [[nodiscard]] bool is_one_of(ObjectType type, const ObjectType* values, std::size_t count) noexcept
        {
            for (std::size_t index = 0; index < count; ++index)
            {
                if (type == values[index])
                {
                    return true;
                }
            }

            return false;
        }

        template <std::size_t Count>
        [[nodiscard]] bool is_one_of(ObjectType type, const ObjectType(&values)[Count]) noexcept
        {
            return is_one_of(type, values, Count);
        }
    }

    bool is_allowed_root_type(ObjectType type) noexcept
    {
        constexpr ObjectType allowed_root_types[] = {
            ObjectType::Site,
            ObjectType::Building,
            ObjectType::Zone,
            ObjectType::Line,
            ObjectType::EquipmentGroup,
            ObjectType::Custom
        };

        return is_one_of(type, allowed_root_types);
    }

    bool is_allowed_child_type(
        ObjectType parent_type,
        ObjectType child_type
    ) noexcept
    {
        if (parent_type == ObjectType::Custom || child_type == ObjectType::Custom)
        {
            return true;
        }

        switch (parent_type)
        {
        case ObjectType::Site:
        {
            constexpr ObjectType allowed[] = {
                ObjectType::Building,
                ObjectType::Zone,
                ObjectType::Line,
                ObjectType::EquipmentGroup
            };

            return is_one_of(child_type, allowed);
        }

        case ObjectType::Building:
        {
            constexpr ObjectType allowed[] = {
                ObjectType::Floor,
                ObjectType::Room,
                ObjectType::Zone,
                ObjectType::Cabinet,
                ObjectType::Rack,
                ObjectType::Line,
                ObjectType::EquipmentGroup
            };

            return is_one_of(child_type, allowed);
        }

        case ObjectType::Floor:
        {
            constexpr ObjectType allowed[] = {
                ObjectType::Room,
                ObjectType::Zone,
                ObjectType::Cabinet,
                ObjectType::Rack,
                ObjectType::EquipmentGroup
            };

            return is_one_of(child_type, allowed);
        }

        case ObjectType::Room:
        {
            constexpr ObjectType allowed[] = {
                ObjectType::Zone,
                ObjectType::Cabinet,
                ObjectType::Rack,
                ObjectType::EquipmentGroup
            };

            return is_one_of(child_type, allowed);
        }

        case ObjectType::Zone:
        {
            constexpr ObjectType allowed[] = {
                ObjectType::Cabinet,
                ObjectType::Rack,
                ObjectType::Line,
                ObjectType::EquipmentGroup
            };

            return is_one_of(child_type, allowed);
        }

        case ObjectType::Cabinet:
        {
            constexpr ObjectType allowed[] = {
                ObjectType::Rack,
                ObjectType::EquipmentGroup
            };

            return is_one_of(child_type, allowed);
        }

        case ObjectType::Rack:
        {
            constexpr ObjectType allowed[] = {
                ObjectType::EquipmentGroup
            };

            return is_one_of(child_type, allowed);
        }

        case ObjectType::Line:
        {
            constexpr ObjectType allowed[] = {
                ObjectType::Cabinet,
                ObjectType::Rack,
                ObjectType::EquipmentGroup
            };

            return is_one_of(child_type, allowed);
        }

        case ObjectType::EquipmentGroup:
        {
            constexpr ObjectType allowed[] = {
                ObjectType::EquipmentGroup
            };

            return is_one_of(child_type, allowed);
        }

        case ObjectType::Custom:
            return true;
        }

        return false;
    }
}
