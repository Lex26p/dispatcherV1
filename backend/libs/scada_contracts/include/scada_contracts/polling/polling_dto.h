#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::contracts::polling
{
    struct PollingAddressDto
    {
        std::string protocol;
        std::string address;
        std::string protocol_address_json;

        std::optional<std::uint16_t> bit_index;
        std::optional<std::uint32_t> byte_offset;
        std::optional<std::uint64_t> mask;
        std::optional<std::uint16_t> shift;
        std::optional<std::uint16_t> bit_length;
    };

    struct PollingTaskDto
    {
        std::string id;
        std::string group_id;

        std::string device_id;
        std::string tag_id;

        std::string protocol;
        PollingAddressDto address;

        std::string state;

        std::uint32_t interval_ms = 1000;
        std::uint32_t timeout_ms = 1000;
        std::uint32_t priority = 100;

        bool is_enabled = true;

        std::string description;
    };

    struct PollingGroupDto
    {
        std::string id;

        std::string device_id;
        std::string protocol;
        std::string mode;

        std::string code;
        std::string name;
        std::string description;

        std::uint32_t interval_ms = 1000;
        std::uint32_t timeout_ms = 1000;
        std::uint32_t max_batch_size = 64;
        std::uint32_t priority = 100;

        bool is_enabled = true;

        std::vector<PollingTaskDto> tasks;
    };

    struct PollingTaskExecutionResultDto
    {
        std::string task_id;
        std::string tag_id;

        std::string resulting_state;
        std::string quality;

        bool success = false;
        std::uint32_t latency_ms = 0;

        std::string message;
    };

    struct PollingGroupExecutionResultDto
    {
        std::string group_id;

        std::string status;

        std::vector<PollingTaskExecutionResultDto> task_results;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
    };

    struct PollingValidationIssueDto
    {
        std::string code;
        std::string entity_id;
        std::string message;
    };

    struct PollingTaskValidationResultDto
    {
        std::vector<PollingValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };

    struct PollingGroupValidationResultDto
    {
        std::vector<PollingValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };
}
