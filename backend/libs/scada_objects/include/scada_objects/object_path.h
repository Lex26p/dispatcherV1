#pragma once

#include "scada_common/id_types.h"
#include "scada_objects/object_node.h"

#include <optional>
#include <string>
#include <vector>

namespace dispatcher::objects
{
    [[nodiscard]] std::optional<ObjectNode> find_object_by_id(
        const std::vector<ObjectNode>& objects,
        const dispatcher::common::Uuid& id
    );

    [[nodiscard]] std::string build_object_path(
        const std::vector<ObjectNode>& objects,
        const dispatcher::common::Uuid& leaf_id
    );
}