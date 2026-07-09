#pragma once

#include "scada_protocols/protocol_driver.h"

#include <atomic>
#include <cstdint>
#include <string_view>

namespace dispatcher::protocols::drivers
{
    class SimulatorProtocolDriver final : public IProtocolDriver
    {
    public:
        SimulatorProtocolDriver() = default;

        [[nodiscard]] dispatcher::devices::DeviceProtocol protocol() const noexcept override;

        [[nodiscard]] std::string_view name() const noexcept override;

        [[nodiscard]] ProtocolCapabilities capabilities() const override;

        [[nodiscard]] ProtocolConnectionCheckResult check_connection(
            const ProtocolConnectionCheckRequest& request
        ) override;

        [[nodiscard]] ProtocolAddressTestResult test_address(
            const ProtocolAddressTestRequest& request
        ) override;

        [[nodiscard]] ProtocolReadResult read(
            const ProtocolReadRequest& request
        ) override;

        [[nodiscard]] ProtocolBatchReadResult read_batch(
            const ProtocolBatchReadRequest& request
        ) override;

        [[nodiscard]] ProtocolWriteResult write(
            const ProtocolWriteRequest& request
        ) override;

    private:
        std::atomic<std::uint64_t> read_counter_ = 0;

        [[nodiscard]] dispatcher::tags::TagValuePayload make_value_for_address(
            std::string_view address,
            std::uint64_t sequence
        ) const;
    };
}