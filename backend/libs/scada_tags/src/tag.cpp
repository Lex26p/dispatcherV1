#include "scada_tags/tag.h"

namespace dispatcher::tags
{
    bool Tag::has_valid_identity() const noexcept
    {
        return !id.empty() && !code.empty() && !name.empty();
    }

    bool Tag::is_device_tag() const noexcept
    {
        return type == TagType::DeviceTag;
    }

    bool Tag::is_assigned_to_device() const noexcept
    {
        return device_id.has_value() && !device_id->empty();
    }

    bool Tag::is_assigned_to_object() const noexcept
    {
        return object_id.has_value() && !object_id->empty();
    }

    bool Tag::requires_device() const noexcept
    {
        return type == TagType::DeviceTag ||
            type == TagType::CommandTag;
    }
}