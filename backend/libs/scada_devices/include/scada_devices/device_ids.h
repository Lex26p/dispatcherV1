#pragma once

#include "scada_common/entity_id.h"

namespace dispatcher::devices
{
    struct DeviceIdTag;

    using DeviceId = dispatcher::common::EntityId<DeviceIdTag>;
}
