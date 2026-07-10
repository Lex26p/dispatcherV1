#include "scada_objects/object_node.h"

namespace dispatcher::objects
{
    bool ObjectNode::is_root() const noexcept
    {
        return !parent_id.has_value();
    }

    bool ObjectNode::has_valid_identity() const noexcept
    {
        return !id.empty() && !code.empty() && !name.empty();
    }
}
