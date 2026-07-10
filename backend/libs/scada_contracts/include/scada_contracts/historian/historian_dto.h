#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace dispatcher::contracts::historian
{
    struct HistorySampleDto
    {
        std::string id;
        std::string tag_id;

        std::string value_type;
        std::string quality;
        std::string source;

        std::string raw_value;
        std::string engineering_value;

        std::string timestamp;
        std::string source_timestamp;
        std::string server_timestamp;

        std::uint64_t change_counter = 0;

        std::string engineering_unit;
        std::string message;

        [[nodiscard]] bool has_sample_id() const noexcept;
        [[nodiscard]] bool has_tag_id() const noexcept;
        [[nodiscard]] bool has_value() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct ArchiveDecisionDto
    {
        std::string tag_id;

        std::string policy;
        std::string reason;

        bool should_archive = false;
        bool first_sample = false;
        bool value_changed = false;
        bool quality_changed = false;
        bool throttled = false;

        std::string message;

        [[nodiscard]] bool should_archive_sample() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct HistoryBatchDto
    {
        std::vector<HistorySampleDto> samples;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::uint64_t size() const noexcept;
    };

    struct HistoryBatchWriteResultDto
    {
        std::string status;

        std::uint64_t attempted_count = 0;
        std::uint64_t written_count = 0;
        std::uint64_t failed_count = 0;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_failures() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct HistoryQueryTimeRangeDto
    {
        std::string from;
        std::string to;

        [[nodiscard]] bool has_from() const noexcept;
        [[nodiscard]] bool has_to() const noexcept;
        [[nodiscard]] bool has_range() const noexcept;
    };

    struct HistoryQueryFilterDto
    {
        std::vector<std::string> tag_ids;

        std::string quality;
        std::string value_type;
        std::string source;

        [[nodiscard]] bool has_tag_filter() const noexcept;
        [[nodiscard]] bool has_quality_filter() const noexcept;
        [[nodiscard]] bool has_value_type_filter() const noexcept;
        [[nodiscard]] bool has_source_filter() const noexcept;
        [[nodiscard]] bool has_any_filter() const noexcept;
    };

    struct HistoryQueryOptionsDto
    {
        std::string order = "Ascending";

        std::uint32_t limit = 1000;
        std::uint32_t offset = 0;
        std::uint32_t max_limit = 10000;

        bool include_raw_value = true;
        bool include_message = true;

        [[nodiscard]] bool has_offset() const noexcept;
    };

    struct HistoryQueryDto
    {
        HistoryQueryTimeRangeDto time_range;
        HistoryQueryFilterDto filter;
        HistoryQueryOptionsDto options;

        [[nodiscard]] bool has_time_range() const noexcept;
        [[nodiscard]] bool has_filters() const noexcept;
    };

    struct HistoryQueryResultDto
    {
        std::vector<HistorySampleDto> samples;

        std::uint64_t total_count = 0;
        bool truncated = false;

        std::string message;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::uint64_t size() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct HistoryValidationIssueDto
    {
        std::string code;
        std::string tag_id;
        std::string message;

        [[nodiscard]] bool has_message() const noexcept;
    };

    struct HistoryValidationResultDto
    {
        std::vector<HistoryValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
        [[nodiscard]] bool has_issues() const noexcept;
    };
}
