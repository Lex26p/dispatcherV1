#pragma once

#include "scada_common/entity_id.h"

namespace dispatcher::tags
{
    struct TagIdTag;

    using TagId = dispatcher::common::EntityId<TagIdTag>;
}