#pragma once

#include "scada_objects/object_ids.h"
#include "scada_objects/object_node.h"
#include "scada_objects/object_tree.h"

#include <optional>
#include <vector>

namespace dispatcher::objects::repository
{
    class IObjectRepository
    {
    public:
        virtual ~IObjectRepository() = default;

        [[nodiscard]] virtual std::vector<ObjectNode> get_all() const = 0;

        [[nodiscard]] virtual std::optional<ObjectNode> find_by_id(
            const ObjectId& id
        ) const = 0;

        [[nodiscard]] virtual std::vector<ObjectNode> find_children(
            const ObjectId& parent_id
        ) const = 0;

        [[nodiscard]] virtual ObjectTree load_tree() const = 0;

        virtual void save(
            const ObjectNode& object
        ) = 0;

        virtual bool remove_by_id(
            const ObjectId& id
        ) = 0;
    };
}