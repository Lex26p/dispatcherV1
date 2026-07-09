#pragma once

#include "scada_devices/device_protocol.h"

#include <cstdint>
#include <optional>
#include <string>

namespace dispatcher::tags
{
    struct TagAddress
    {
        dispatcher::devices::DeviceProtocol protocol = dispatcher::devices::DeviceProtocol::Custom;

        std::string address;
        std::string protocol_address_json;

        std::optional<std::uint16_t> bit_index;
        std::optional<std::uint32_t> byte_offset;
        std::optional<std::uint64_t> mask;
        std::optional<std::uint8_t> shift;
        std::optional<std::uint8_t> bit_length;

        [[nodiscard]] bool has_address() const noexcept;
        [[nodiscard]] bool has_protocol_payload() const noexcept;
        [[nodiscard]] bool uses_bit_extraction() const noexcept;
    };
}