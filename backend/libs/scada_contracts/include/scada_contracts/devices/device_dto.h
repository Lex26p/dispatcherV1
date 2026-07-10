#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::contracts::devices
{
    struct DeviceConnectionDto
    {
        std::string protocol;
        std::string host;
        std::uint16_t port = 0;
        std::string ip_mode;
        std::string network_interface;
    };

    struct DeviceDto
    {
        std::string id;
        std::optional<std::string> object_id;

        std::string code;
        std::string name;
        std::string description;

        std::string protocol;
        std::string state;

        DeviceConnectionDto connection;
    };

    struct DeviceListDto
    {
        std::vector<DeviceDto> devices;
    };

    struct DeviceValidationIssueDto
    {
        std::string code;
        std::string device_id;
        std::string message;
    };

    struct DeviceValidationResultDto
    {
        std::vector<DeviceValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };

    struct DeviceDiagnosticsDto
    {
        std::string device_id;

        std::string communication_status;
        std::string health_status;

        std::uint64_t successful_requests = 0;
        std::uint64_t failed_requests = 0;
        std::uint64_t timeout_count = 0;
        std::uint64_t reconnect_count = 0;

        std::uint32_t last_latency_ms = 0;
        std::string last_error;
    };
}
