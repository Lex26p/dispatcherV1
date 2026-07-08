#pragma once

#include <string_view>

namespace dispatcher::devices
{
    enum class DeviceProtocol
    {
        ModbusTcp,
        Snmp,
        Simulator,
        Custom
    };

    [[nodiscard]] std::string_view to_string(DeviceProtocol protocol) noexcept;
}