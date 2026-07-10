#pragma once

#include "scada_devices/device_ids.h"
#include "scada_objects/object_ids.h"
#include "scada_tags/tag_address.h"
#include "scada_tags/tag_archive_policy.h"
#include "scada_tags/tag_ids.h"
#include "scada_tags/tag_type.h"
#include "scada_tags/tag_value_type.h"

#include <optional>
#include <string>

namespace dispatcher::tags
{
    struct Tag
    {
        TagId id;

        std::optional<dispatcher::objects::ObjectId> object_id;
        std::optional<dispatcher::devices::DeviceId> device_id;

        TagType type = TagType::DeviceTag;
        TagValueType value_type = TagValueType::Double;
        TagArchivePolicy archive_policy = TagArchivePolicy::Disabled;

        std::string code;
        std::string name;
        std::string description;

        std::string engineering_unit;
        double scale = 1.0;
        double offset = 0.0;

        TagAddress address;

        bool is_enabled = true;

        [[nodiscard]] bool has_valid_identity() const noexcept;
        [[nodiscard]] bool is_device_tag() const noexcept;
        [[nodiscard]] bool is_assigned_to_device() const noexcept;
        [[nodiscard]] bool is_assigned_to_object() const noexcept;
        [[nodiscard]] bool requires_device() const noexcept;
    };
}
