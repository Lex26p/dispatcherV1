#pragma once

#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"

#include <optional>
#include <vector>

namespace dispatcher::tags::repository
{
    class ITagCurrentValueRepository
    {
    public:
        virtual ~ITagCurrentValueRepository() = default;

        [[nodiscard]] virtual std::vector<TagCurrentValue> get_all() const = 0;

        [[nodiscard]] virtual std::optional<TagCurrentValue> find_by_tag_id(
            const TagId& tag_id
        ) const = 0;

        virtual void save(
            const TagCurrentValue& value
        ) = 0;

        virtual bool remove_by_tag_id(
            const TagId& tag_id
        ) = 0;

        virtual void clear() = 0;
    };
}