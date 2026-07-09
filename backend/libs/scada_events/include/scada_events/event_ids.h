#pragma once

#include "scada_common/entity_id.h"

namespace dispatcher::events
{
    struct EventIdTag;

    using EventId = dispatcher::common::EntityId<EventIdTag>;
}