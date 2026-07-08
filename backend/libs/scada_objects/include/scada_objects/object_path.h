#pragma once

#include "scada_objects/object_ids.h"
#include "scada_objects/object_node.h"

#include <optional>
#include <string>
#include <vector>

namespace dispatcher::objects
{
    [[nodiscard]] std::optional<ObjectNode> find_object_by_id(
        const std::vector<ObjectNode>& objects,
        const ObjectId& id
    );

    [[nodiscard]] std::string build_object_path(
        const std::vector<ObjectNode>& objects,
        const ObjectId& leaf_id
    );
}