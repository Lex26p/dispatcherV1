#pragma once

#include "scada_runtime/runtime_event.h"
#include "scada_tags/tag_ids.h"

#include <vector>

namespace dispatcher::runtime::repository
{
    class IRuntimeEventRepository
    {
    public:
        virtual ~IRuntimeEventRepository() = default;

        [[nodiscard]] virtual std::vector<RuntimeValueEvent> get_all() const = 0;

        [[nodiscard]] virtual std::vector<RuntimeValueEvent> find_by_tag_id(
            const dispatcher::tags::TagId& tag_id
        ) const = 0;

        [[nodiscard]] virtual std::vector<RuntimeValueEvent> get_latest(
            std::size_t limit
        ) const = 0;

        virtual void save(
            const RuntimeValueEvent& event
        ) = 0;

        virtual void clear() = 0;
    };
}
