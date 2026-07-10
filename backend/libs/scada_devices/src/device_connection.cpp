#include "scada_devices/device_connection.h"

namespace dispatcher::devices
{
    bool DeviceConnection::has_valid_endpoint() const noexcept
    {
        return !host.empty() && port > 0;
    }

    bool DeviceConnection::has_network_interface() const noexcept
    {
        return !network_interface.empty();
    }

    std::string_view to_string(IpMode mode) noexcept
    {
        switch (mode)
        {
        case IpMode::IPv4:
            return "IPv4";
        case IpMode::IPv6:
            return "IPv6";
        case IpMode::Auto:
            return "Auto";
        }

        return "Unknown";
    }
}
