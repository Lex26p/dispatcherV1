#pragma once

#include "scada_common/entity_id.h"

namespace dispatcher::alarms
{
    struct AlarmIdTag;

    using AlarmId = dispatcher::common::EntityId<AlarmIdTag>;
}