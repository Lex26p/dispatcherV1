#pragma once

#include "scada_devices/device_protocol.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace dispatcher::devices
{
    enum class IpMode
    {
        IPv4,
        IPv6,
        Auto
    };

    struct DeviceConnection
    {
        DeviceProtocol protocol = DeviceProtocol::Custom;

        std::string host;
        std::uint16_t port = 0;
        IpMode ip_mode = IpMode::Auto;
        std::string network_interface;

        [[nodiscard]] bool has_valid_endpoint() const noexcept;
        [[nodiscard]] bool has_network_interface() const noexcept;
    };

    [[nodiscard]] std::string_view to_string(IpMode mode) noexcept;
}
