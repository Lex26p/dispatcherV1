#pragma once

#include "scada_devices/device_ids.h"
#include "scada_objects/object_ids.h"
#include "scada_tags/tag.h"
#include "scada_tags/tag_archive_policy.h"
#include "scada_tags/tag_ids.h"
#include "scada_tags/tag_type.h"

#include <optional>
#include <string_view>
#include <vector>

namespace dispatcher::tags::repository
{
    class ITagRepository
    {
    public:
        virtual ~ITagRepository() = default;

        [[nodiscard]] virtual std::vector<Tag> get_all() const = 0;

        [[nodiscard]] virtual std::optional<Tag> find_by_id(
            const TagId& id
        ) const = 0;

        [[nodiscard]] virtual std::optional<Tag> find_by_code(
            std::string_view code
        ) const = 0;

        [[nodiscard]] virtual std::vector<Tag> find_by_object_id(
            const dispatcher::objects::ObjectId& object_id
        ) const = 0;

        [[nodiscard]] virtual std::vector<Tag> find_by_device_id(
            const dispatcher::devices::DeviceId& device_id
        ) const = 0;

        [[nodiscard]] virtual std::vector<Tag> find_by_type(
            TagType type
        ) const = 0;

        [[nodiscard]] virtual std::vector<Tag> find_by_archive_policy(
            TagArchivePolicy archive_policy
        ) const = 0;

        virtual void save(
            const Tag& tag
        ) = 0;

        virtual bool remove_by_id(
            const TagId& id
        ) = 0;
    };
}