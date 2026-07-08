#include "scada_devices/device.h"

namespace dispatcher::devices
{
    bool Device::has_valid_identity() const noexcept
    {
        return !id.empty() && !code.empty() && !name.empty();
    }

    bool Device::is_assigned_to_object() const noexcept
    {
        return object_id.has_value() && !object_id->empty();
    }

    bool Device::is_enabled_for_runtime() const noexcept
    {
        return state == DeviceState::Active || state == DeviceState::Simulation;
    }
}