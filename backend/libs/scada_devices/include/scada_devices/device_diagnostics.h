#pragma once

#include "scada_devices/device_ids.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace dispatcher::devices
{
    enum class DeviceCommunicationStatus
    {
        Unknown,
        NotConfigured,
        Online,
        Degraded,
        Offline,
        Timeout,
        ProtocolError,
        Disabled
    };

    enum class DeviceHealthStatus
    {
        Unknown,
        Healthy,
        Warning,
        Critical,
        Offline,
        Disabled
    };

    struct DeviceDiagnostics
    {
        DeviceId device_id;

        DeviceCommunicationStatus communication_status = DeviceCommunicationStatus::Unknown;
        DeviceHealthStatus health_status = DeviceHealthStatus::Unknown;

        std::uint64_t successful_requests = 0;
        std::uint64_t failed_requests = 0;
        std::uint64_t timeout_count = 0;
        std::uint64_t reconnect_count = 0;

        std::uint32_t last_latency_ms = 0;
        std::string last_error;

        [[nodiscard]] std::uint64_t total_requests() const noexcept;
        [[nodiscard]] bool has_successful_communication() const noexcept;
        [[nodiscard]] bool has_failures() const noexcept;
        [[nodiscard]] bool is_online() const noexcept;
        [[nodiscard]] bool requires_attention() const noexcept;
    };

    [[nodiscard]] std::string_view to_string(
        DeviceCommunicationStatus status
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        DeviceHealthStatus status
    ) noexcept;
}