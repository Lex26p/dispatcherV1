#pragma once

#include "scada_common/entity_id.h"

namespace dispatcher::alarms
{
    struct AlarmIdTag;
    struct AlarmRuleIdTag;

    using AlarmId = dispatcher::common::EntityId<AlarmIdTag>;
    using AlarmRuleId = dispatcher::common::EntityId<AlarmRuleIdTag>;
}