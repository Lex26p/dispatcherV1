#include "scada_devices/device_protocol.h"

namespace dispatcher::devices
{
    std::string_view to_string(DeviceProtocol protocol) noexcept
    {
        switch (protocol)
        {
        case DeviceProtocol::ModbusTcp:
            return "ModbusTcp";
        case DeviceProtocol::Snmp:
            return "Snmp";
        case DeviceProtocol::Simulator:
            return "Simulator";
        case DeviceProtocol::Custom:
            return "Custom";
        }

        return "Unknown";
    }
}