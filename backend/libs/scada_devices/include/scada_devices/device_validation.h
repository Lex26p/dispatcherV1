#pragma once

#include "scada_devices/device.h"
#include "scada_devices/device_protocol.h"
#include "scada_devices/device_state.h"

#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::devices
{
    enum class DeviceValidationCode
    {
        EmptyIdentity,
        MissingObjectAssignment,
        ProtocolMismatch,
        MissingHost,
        InvalidPort,
        RuntimeEndpointRequired,
        UnsupportedRuntimeState
    };

    struct DeviceValidationIssue
    {
        DeviceValidationCode code = DeviceValidationCode::EmptyIdentity;
        DeviceId device_id;
        std::string message;
    };

    struct DeviceValidationResult
    {
        std::vector<DeviceValidationIssue> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };

    [[nodiscard]] bool requires_network_endpoint(
        DeviceProtocol protocol
    ) noexcept;

    [[nodiscard]] bool is_runtime_enabled_state(
        DeviceState state
    ) noexcept;

    [[nodiscard]] DeviceValidationResult validate_device(
        const Device& device
    );

    [[nodiscard]] std::string_view to_string(
        DeviceValidationCode code
    ) noexcept;
}