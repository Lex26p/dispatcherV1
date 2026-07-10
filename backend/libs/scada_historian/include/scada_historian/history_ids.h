#pragma once

#include "scada_common/entity_id.h"

namespace dispatcher::historian
{
    struct HistorySampleIdTag;

    using HistorySampleId = dispatcher::common::EntityId<HistorySampleIdTag>;
}
