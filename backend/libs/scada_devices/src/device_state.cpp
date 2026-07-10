#include "scada_devices/device_state.h"

namespace dispatcher::devices
{
    std::string_view to_string(DeviceState state) noexcept
    {
        switch (state)
        {
        case DeviceState::Draft:
            return "Draft";
        case DeviceState::Commissioning:
            return "Commissioning";
        case DeviceState::Active:
            return "Active";
        case DeviceState::Maintenance:
            return "Maintenance";
        case DeviceState::Disabled:
            return "Disabled";
        case DeviceState::OutOfService:
            return "OutOfService";
        case DeviceState::Decommissioned:
            return "Decommissioned";
        case DeviceState::Simulation:
            return "Simulation";
        }

        return "Unknown";
    }
}
