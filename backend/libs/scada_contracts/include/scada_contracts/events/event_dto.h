#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace dispatcher::contracts::events
{
    struct EventRecordDto
    {
        std::string id;

        std::string category;
        std::string severity;
        std::string source_type;

        std::string code;
        std::string title;
        std::string message;

        std::string source_id;
        std::string object_id;
        std::string device_id;
        std::string tag_id;

        std::string correlation_id;

        std::string timestamp;

        std::uint64_t sequence_number = 0;

        [[nodiscard]] bool has_event_id() const noexcept;
        [[nodiscard]] bool has_code() const noexcept;
        [[nodiscard]] bool has_title() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
        [[nodiscard]] bool has_source_id() const noexcept;
        [[nodiscard]] bool has_object_id() const noexcept;
        [[nodiscard]] bool has_device_id() const noexcept;
        [[nodiscard]] bool has_tag_id() const noexcept;
        [[nodiscard]] bool has_correlation_id() const noexcept;
        [[nodiscard]] bool has_timestamp() const noexcept;
    };

    struct EventListDto
    {
        std::vector<EventRecordDto> events;

        std::uint64_t total_count = 0;
        bool truncated = false;

        std::string message;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::uint64_t size() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct EventValidationIssueDto
    {
        std::string code;
        std::string event_id;
        std::string message;

        [[nodiscard]] bool has_message() const noexcept;
    };

    struct EventValidationResultDto
    {
        std::vector<EventValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
        [[nodiscard]] bool has_issues() const noexcept;
    };
}
