#pragma once

#include "scada_common/entity_id.h"

namespace dispatcher::objects
{
    struct ObjectIdTag;
    struct ResponsibilityZoneIdTag;

    using ObjectId = dispatcher::common::EntityId<ObjectIdTag>;
    using ResponsibilityZoneId = dispatcher::common::EntityId<ResponsibilityZoneIdTag>;
}