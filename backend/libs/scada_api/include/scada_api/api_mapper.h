#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::api
{
    using ApiTimestamp = std::chrono::system_clock::time_point;

    enum class ApiMappingStatus
    {
        NotMapped,
        Mapped,
        Skipped,
        InvalidInput,
        UnsupportedType,
        Failed
    };

    enum class ApiTimestampFormat
    {
        Iso8601Utc,
        UnixMilliseconds,
        EmptyForDefault
    };

    struct ApiMapperOptions
    {
        ApiTimestampFormat timestamp_format = ApiTimestampFormat::Iso8601Utc;

        bool include_empty_fields = true;
        bool include_internal_ids = true;
        bool strict = false;
    };

    struct ApiMappingIssue
    {
        ApiMappingStatus status = ApiMappingStatus::NotMapped;

        std::string field;
        std::string message;

        [[nodiscard]] bool has_field() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct ApiMappingResult
    {
        ApiMappingStatus status = ApiMappingStatus::NotMapped;

        std::vector<ApiMappingIssue> issues;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_issues() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    [[nodiscard]] ApiMappingResult make_mapping_success(
        std::string message = {}
    );

    [[nodiscard]] ApiMappingResult make_mapping_error(
        ApiMappingStatus status,
        std::string field,
        std::string message
    );

    [[nodiscard]] bool is_empty_timestamp(
        ApiTimestamp timestamp
    ) noexcept;

    [[nodiscard]] std::string format_timestamp(
        ApiTimestamp timestamp,
        ApiTimestampFormat format = ApiTimestampFormat::Iso8601Utc
    );

    [[nodiscard]] std::string format_bool(
        bool value
    );

    [[nodiscard]] std::string format_uint64(
        std::uint64_t value
    );

    [[nodiscard]] std::string_view to_string(
        ApiMappingStatus status
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        ApiTimestampFormat format
    ) noexcept;
}