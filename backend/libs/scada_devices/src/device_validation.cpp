#include "scada_devices/device_validation.h"

namespace dispatcher::devices
{
    bool DeviceValidationResult::is_valid() const noexcept
    {
        return issues.empty();
    }

    bool requires_network_endpoint(
        DeviceProtocol protocol
    ) noexcept
    {
        switch (protocol)
        {
        case DeviceProtocol::ModbusTcp:
            return true;
        case DeviceProtocol::Snmp:
            return true;
        case DeviceProtocol::Custom:
            return true;
        case DeviceProtocol::Simulator:
            return false;
        }

        return true;
    }

    bool is_runtime_enabled_state(
        DeviceState state
    ) noexcept
    {
        return state == DeviceState::Active || state == DeviceState::Simulation;
    }

    DeviceValidationResult validate_device(
        const Device& device
    )
    {
        DeviceValidationResult result;

        if (!device.has_valid_identity())
        {
            result.issues.push_back(
                DeviceValidationIssue{
                    .code = DeviceValidationCode::EmptyIdentity,
                    .device_id = device.id,
                    .message = "Device identity is incomplete. Required fields: id, code, name."
                }
            );
        }

        if (device.state != DeviceState::Draft && !device.is_assigned_to_object())
        {
            result.issues.push_back(
                DeviceValidationIssue{
                    .code = DeviceValidationCode::MissingObjectAssignment,
                    .device_id = device.id,
                    .message = "Device must be assigned to object before leaving Draft state."
                }
            );
        }

        if (device.protocol != device.connection.protocol)
        {
            result.issues.push_back(
                DeviceValidationIssue{
                    .code = DeviceValidationCode::ProtocolMismatch,
                    .device_id = device.id,
                    .message = "Device protocol and connection protocol must match."
                }
            );
        }

        if (is_runtime_enabled_state(device.state) && !device.is_enabled_for_runtime())
        {
            result.issues.push_back(
                DeviceValidationIssue{
                    .code = DeviceValidationCode::UnsupportedRuntimeState,
                    .device_id = device.id,
                    .message = "Device state is not supported for runtime."
                }
            );
        }

        const auto endpoint_required = requires_network_endpoint(device.protocol);

        if (endpoint_required && device.connection.host.empty())
        {
            result.issues.push_back(
                DeviceValidationIssue{
                    .code = DeviceValidationCode::MissingHost,
                    .device_id = device.id,
                    .message = "Device connection host is required for selected protocol."
                }
            );
        }

        if (endpoint_required && device.connection.port == 0)
        {
            result.issues.push_back(
                DeviceValidationIssue{
                    .code = DeviceValidationCode::InvalidPort,
                    .device_id = device.id,
                    .message = "Device connection port must be greater than zero for selected protocol."
                }
            );
        }

        if (is_runtime_enabled_state(device.state) &&
            endpoint_required &&
            !device.connection.has_valid_endpoint())
        {
            result.issues.push_back(
                DeviceValidationIssue{
                    .code = DeviceValidationCode::RuntimeEndpointRequired,
                    .device_id = device.id,
                    .message = "Runtime-enabled device must have valid connection endpoint."
                }
            );
        }

        return result;
    }

    std::string_view to_string(
        DeviceValidationCode code
    ) noexcept
    {
        switch (code)
        {
        case DeviceValidationCode::EmptyIdentity:
            return "EmptyIdentity";
        case DeviceValidationCode::MissingObjectAssignment:
            return "MissingObjectAssignment";
        case DeviceValidationCode::ProtocolMismatch:
            return "ProtocolMismatch";
        case DeviceValidationCode::MissingHost:
            return "MissingHost";
        case DeviceValidationCode::InvalidPort:
            return "InvalidPort";
        case DeviceValidationCode::RuntimeEndpointRequired:
            return "RuntimeEndpointRequired";
        case DeviceValidationCode::UnsupportedRuntimeState:
            return "UnsupportedRuntimeState";
        }

        return "Unknown";
    }
}
