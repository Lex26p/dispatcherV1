#pragma once

#include "scada_devices/device_connection.h"
#include "scada_devices/device_ids.h"
#include "scada_tags/tag_address.h"
#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"

#include <cstdint>
#include <string>
#include <vector>

namespace dispatcher::protocols
{
    struct ProtocolConnectionCheckRequest
    {
        dispatcher::devices::DeviceId device_id;
        dispatcher::devices::DeviceConnection connection;

        std::uint32_t timeout_ms = 1000;
        std::string correlation_id;
    };

    struct ProtocolAddressTestRequest
    {
        dispatcher::devices::DeviceId device_id;
        dispatcher::devices::DeviceConnection connection;
        dispatcher::tags::TagAddress address;

        std::uint32_t timeout_ms = 1000;
        std::string correlation_id;
    };

    struct ProtocolReadRequest
    {
        dispatcher::devices::DeviceId device_id;
        dispatcher::devices::DeviceConnection connection;

        dispatcher::tags::TagId tag_id;
        dispatcher::tags::TagAddress address;

        std::uint32_t timeout_ms = 1000;
        std::string correlation_id;
    };

    struct ProtocolBatchReadRequest
    {
        dispatcher::devices::DeviceId device_id;
        dispatcher::devices::DeviceConnection connection;

        std::vector<ProtocolReadRequest> items;

        std::uint32_t timeout_ms = 1000;
        std::string correlation_id;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;
    };

    struct ProtocolWriteRequest
    {
        dispatcher::devices::DeviceId device_id;
        dispatcher::devices::DeviceConnection connection;

        dispatcher::tags::TagId tag_id;
        dispatcher::tags::TagAddress address;
        dispatcher::tags::TagValuePayload value;

        bool dry_run = false;

        std::uint32_t timeout_ms = 1000;
        std::string correlation_id;
    };
}