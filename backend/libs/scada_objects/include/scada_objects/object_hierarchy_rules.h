#pragma once

#include "scada_objects/object_type.h"

namespace dispatcher::objects
{
    [[nodiscard]] bool is_allowed_root_type(ObjectType type) noexcept;

    [[nodiscard]] bool is_allowed_child_type(
        ObjectType parent_type,
        ObjectType child_type
    ) noexcept;
}