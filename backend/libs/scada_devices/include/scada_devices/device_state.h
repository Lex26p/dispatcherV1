#pragma once

#include <string_view>

namespace dispatcher::devices
{
    enum class DeviceState
    {
        Draft,
        Commissioning,
        Active,
        Maintenance,
        Disabled,
        OutOfService,
        Decommissioned,
        Simulation
    };

    [[nodiscard]] std::string_view to_string(DeviceState state) noexcept;
}
