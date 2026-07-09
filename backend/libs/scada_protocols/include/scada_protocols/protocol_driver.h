#pragma once

#include "scada_devices/device_protocol.h"
#include "scada_protocols/protocol_capabilities.h"
#include "scada_protocols/protocol_requests.h"
#include "scada_protocols/protocol_results.h"

#include <string_view>

namespace dispatcher::protocols
{
    class IProtocolDriver
    {
    public:
        virtual ~IProtocolDriver() = default;

        [[nodiscard]] virtual dispatcher::devices::DeviceProtocol protocol() const noexcept = 0;

        [[nodiscard]] virtual std::string_view name() const noexcept = 0;

        [[nodiscard]] virtual ProtocolCapabilities capabilities() const = 0;

        [[nodiscard]] virtual ProtocolConnectionCheckResult check_connection(
            const ProtocolConnectionCheckRequest& request
        ) = 0;

        [[nodiscard]] virtual ProtocolAddressTestResult test_address(
            const ProtocolAddressTestRequest& request
        ) = 0;

        [[nodiscard]] virtual ProtocolReadResult read(
            const ProtocolReadRequest& request
        ) = 0;

        [[nodiscard]] virtual ProtocolBatchReadResult read_batch(
            const ProtocolBatchReadRequest& request
        ) = 0;

        [[nodiscard]] virtual ProtocolWriteResult write(
            const ProtocolWriteRequest& request
        ) = 0;
    };
}