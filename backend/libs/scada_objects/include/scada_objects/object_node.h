#pragma once

#include "scada_common/id_types.h"
#include "scada_objects/object_type.h"

#include <optional>
#include <string>

namespace dispatcher::objects
{
    struct ObjectNode
    {
        dispatcher::common::Uuid id;
        std::optional<dispatcher::common::Uuid> parent_id;
        ObjectType type = ObjectType::Custom;
        std::string code;
        std::string name;
        std::string description;

        [[nodiscard]] bool is_root() const noexcept;
        [[nodiscard]] bool has_valid_identity() const noexcept;
    };
}