#include "scada_runtime/tag_value_store.h"

#include <utility>

namespace dispatcher::runtime
{
    bool TagValueStore::empty() const noexcept
    {
        return values_.empty();
    }

    std::size_t TagValueStore::size() const noexcept
    {
        return values_.size();
    }

    bool TagValueStore::contains(
        const dispatcher::tags::TagId& tag_id
    ) const
    {
        if (tag_id.empty())
        {
            return false;
        }

        return values_.contains(key_for(tag_id));
    }

    std::optional<dispatcher::tags::TagCurrentValue> TagValueStore::find_by_tag_id(
        const dispatcher::tags::TagId& tag_id
    ) const
    {
        if (tag_id.empty())
        {
            return std::nullopt;
        }

        const auto it = values_.find(key_for(tag_id));

        if (it == values_.end())
        {
            return std::nullopt;
        }

        return it->second;
    }

    std::vector<dispatcher::tags::TagCurrentValue> TagValueStore::get_all() const
    {
        std::vector<dispatcher::tags::TagCurrentValue> result;
        result.reserve(values_.size());

        for (const auto& item : values_)
        {
            result.push_back(item.second);
        }

        return result;
    }

    bool TagValueStore::upsert(
        const dispatcher::tags::TagCurrentValue& value
    )
    {
        if (value.tag_id.empty())
        {
            return false;
        }

        values_.insert_or_assign(
            key_for(value.tag_id),
            value
        );

        return true;
    }

    bool TagValueStore::remove_by_tag_id(
        const dispatcher::tags::TagId& tag_id
    )
    {
        if (tag_id.empty())
        {
            return false;
        }

        return values_.erase(key_for(tag_id)) > 0;
    }

    void TagValueStore::clear() noexcept
    {
        values_.clear();
    }

    std::string TagValueStore::key_for(
        const dispatcher::tags::TagId& tag_id
    )
    {
        return tag_id.str();
    }
}