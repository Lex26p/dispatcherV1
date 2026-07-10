#include "scada_devices/device_diagnostics.h"

namespace dispatcher::devices
{
    std::uint64_t DeviceDiagnostics::total_requests() const noexcept
    {
        return successful_requests + failed_requests;
    }

    bool DeviceDiagnostics::has_successful_communication() const noexcept
    {
        return successful_requests > 0;
    }

    bool DeviceDiagnostics::has_failures() const noexcept
    {
        return failed_requests > 0 || timeout_count > 0 || !last_error.empty();
    }

    bool DeviceDiagnostics::is_online() const noexcept
    {
        return communication_status == DeviceCommunicationStatus::Online ||
            communication_status == DeviceCommunicationStatus::Degraded;
    }

    bool DeviceDiagnostics::requires_attention() const noexcept
    {
        return health_status == DeviceHealthStatus::Warning ||
            health_status == DeviceHealthStatus::Critical ||
            communication_status == DeviceCommunicationStatus::Degraded ||
            communication_status == DeviceCommunicationStatus::Offline ||
            communication_status == DeviceCommunicationStatus::Timeout ||
            communication_status == DeviceCommunicationStatus::ProtocolError ||
            has_failures();
    }

    std::string_view to_string(
        DeviceCommunicationStatus status
    ) noexcept
    {
        switch (status)
        {
        case DeviceCommunicationStatus::Unknown:
            return "Unknown";
        case DeviceCommunicationStatus::NotConfigured:
            return "NotConfigured";
        case DeviceCommunicationStatus::Online:
            return "Online";
        case DeviceCommunicationStatus::Degraded:
            return "Degraded";
        case DeviceCommunicationStatus::Offline:
            return "Offline";
        case DeviceCommunicationStatus::Timeout:
            return "Timeout";
        case DeviceCommunicationStatus::ProtocolError:
            return "ProtocolError";
        case DeviceCommunicationStatus::Disabled:
            return "Disabled";
        }

        return "Unknown";
    }

    std::string_view to_string(
        DeviceHealthStatus status
    ) noexcept
    {
        switch (status)
        {
        case DeviceHealthStatus::Unknown:
            return "Unknown";
        case DeviceHealthStatus::Healthy:
            return "Healthy";
        case DeviceHealthStatus::Warning:
            return "Warning";
        case DeviceHealthStatus::Critical:
            return "Critical";
        case DeviceHealthStatus::Offline:
            return "Offline";
        case DeviceHealthStatus::Disabled:
            return "Disabled";
        }

        return "Unknown";
    }
}
