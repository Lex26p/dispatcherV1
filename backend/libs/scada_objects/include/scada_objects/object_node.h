#pragma once

#include "scada_objects/object_ids.h"
#include "scada_objects/object_type.h"

#include <optional>
#include <string>

namespace dispatcher::objects
{
    struct ObjectNode
    {
        ObjectId id;
        std::optional<ObjectId> parent_id;
        ObjectType type = ObjectType::Custom;
        std::string code;
        std::string name;
        std::string description;

        [[nodiscard]] bool is_root() const noexcept;
        [[nodiscard]] bool has_valid_identity() const noexcept;
    };
}