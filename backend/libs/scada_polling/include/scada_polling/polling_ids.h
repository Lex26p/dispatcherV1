#pragma once

#include "scada_common/entity_id.h"

namespace dispatcher::polling
{
    struct PollingTaskIdTag;
    struct PollingGroupIdTag;

    using PollingTaskId = dispatcher::common::EntityId<PollingTaskIdTag>;
    using PollingGroupId = dispatcher::common::EntityId<PollingGroupIdTag>;
}
