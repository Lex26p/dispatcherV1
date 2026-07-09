#pragma once

#include "scada_devices/device_protocol.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::protocols
{
    struct ProtocolCapabilities
    {
        dispatcher::devices::DeviceProtocol protocol = dispatcher::devices::DeviceProtocol::Custom;

        bool supports_read = true;
        bool supports_write = false;
        bool supports_batch_read = true;
        bool supports_connection_check = true;
        bool supports_address_test = true;
        bool supports_subscriptions = false;

        std::uint32_t max_batch_size = 1;
        std::uint32_t default_timeout_ms = 1000;

        std::vector<std::string> supported_value_types;

        [[nodiscard]] bool can_read() const noexcept;
        [[nodiscard]] bool can_write() const noexcept;
        [[nodiscard]] bool can_batch_read() const noexcept;
        [[nodiscard]] bool can_check_connection() const noexcept;
        [[nodiscard]] bool can_test_address() const noexcept;
    };

    [[nodiscard]] std::string_view to_string(
        const ProtocolCapabilities& capabilities
    ) noexcept;
}