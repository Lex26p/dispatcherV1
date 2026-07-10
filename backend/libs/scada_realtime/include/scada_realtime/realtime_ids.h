#pragma once

#include "scada_common/entity_id.h"

namespace dispatcher::realtime
{
    struct RealtimeMessageIdTag;
    struct RealtimeClientIdTag;
    struct RealtimeSubscriptionIdTag;

    using RealtimeMessageId = dispatcher::common::EntityId<RealtimeMessageIdTag>;
    using RealtimeClientId = dispatcher::common::EntityId<RealtimeClientIdTag>;
    using RealtimeSubscriptionId = dispatcher::common::EntityId<RealtimeSubscriptionIdTag>;
}
