#pragma once

#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::runtime
{
    class TagValueStore
    {
    public:
        [[nodiscard]] bool empty() const noexcept;

        [[nodiscard]] std::size_t size() const noexcept;

        [[nodiscard]] bool contains(
            const dispatcher::tags::TagId& tag_id
        ) const;

        [[nodiscard]] std::optional<dispatcher::tags::TagCurrentValue> find_by_tag_id(
            const dispatcher::tags::TagId& tag_id
        ) const;

        [[nodiscard]] std::vector<dispatcher::tags::TagCurrentValue> get_all() const;

        bool upsert(
            const dispatcher::tags::TagCurrentValue& value
        );

        bool remove_by_tag_id(
            const dispatcher::tags::TagId& tag_id
        );

        void clear() noexcept;

    private:
        std::map<std::string, dispatcher::tags::TagCurrentValue> values_;

        [[nodiscard]] static std::string key_for(
            const dispatcher::tags::TagId& tag_id
        );
    };
}