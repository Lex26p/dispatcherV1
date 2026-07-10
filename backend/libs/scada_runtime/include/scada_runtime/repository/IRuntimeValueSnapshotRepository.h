#pragma once

#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"

#include <optional>
#include <vector>

namespace dispatcher::runtime::repository
{
    class IRuntimeValueSnapshotRepository
    {
    public:
        virtual ~IRuntimeValueSnapshotRepository() = default;

        [[nodiscard]] virtual std::vector<dispatcher::tags::TagCurrentValue> get_all() const = 0;

        [[nodiscard]] virtual std::optional<dispatcher::tags::TagCurrentValue> find_by_tag_id(
            const dispatcher::tags::TagId& tag_id
        ) const = 0;

        virtual void save(
            const dispatcher::tags::TagCurrentValue& value
        ) = 0;

        virtual bool remove_by_tag_id(
            const dispatcher::tags::TagId& tag_id
        ) = 0;

        virtual void clear() = 0;
    };
}
