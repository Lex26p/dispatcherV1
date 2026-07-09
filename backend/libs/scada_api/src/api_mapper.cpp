#include "scada_api/api_mapper.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <utility>

namespace dispatcher::api
{
    namespace
    {
        [[nodiscard]] std::tm to_utc_tm(
            std::time_t time
        )
        {
            std::tm tm{};

#if defined(_WIN32)
            gmtime_s(&tm, &time);
#else
            gmtime_r(&time, &tm);
#endif

            return tm;
        }

        [[nodiscard]] std::int64_t timestamp_to_unix_milliseconds(
            ApiTimestamp timestamp
        )
        {
            const auto duration = timestamp.time_since_epoch();

            return std::chrono::duration_cast<std::chrono::milliseconds>(
                duration
            ).count();
        }
    }

    bool ApiMappingIssue::has_field() const noexcept
    {
        return !field.empty();
    }

    bool ApiMappingIssue::has_message() const noexcept
    {
        return !message.empty();
    }

    bool ApiMappingResult::is_success() const noexcept
    {
        return status == ApiMappingStatus::Mapped;
    }

    bool ApiMappingResult::has_issues() const noexcept
    {
        return !issues.empty();
    }

    bool ApiMappingResult::has_message() const noexcept
    {
        return !message.empty();
    }

    ApiMappingResult make_mapping_success(
        std::string message
    )
    {
        ApiMappingResult result;

        result.status = ApiMappingStatus::Mapped;
        result.message = std::move(message);

        return result;
    }

    ApiMappingResult make_mapping_error(
        ApiMappingStatus status,
        std::string field,
        std::string message
    )
    {
        ApiMappingResult result;

        result.status = status;
        result.message = message;

        result.issues.push_back(
            ApiMappingIssue{
                .status = status,
                .field = std::move(field),
                .message = std::move(message)
            }
        );

        return result;
    }

    bool is_empty_timestamp(
        ApiTimestamp timestamp
    ) noexcept
    {
        return timestamp == ApiTimestamp{};
    }

    std::string format_timestamp(
        ApiTimestamp timestamp,
        ApiTimestampFormat format
    )
    {
        if (is_empty_timestamp(timestamp))
        {
            return {};
        }

        switch (format)
        {
        case ApiTimestampFormat::UnixMilliseconds:
            return std::to_string(
                timestamp_to_unix_milliseconds(timestamp)
            );

        case ApiTimestampFormat::EmptyForDefault:
            return {};

        case ApiTimestampFormat::Iso8601Utc:
            break;
        }

        const auto time = std::chrono::system_clock::to_time_t(timestamp);
        const auto tm = to_utc_tm(time);

        const auto duration = timestamp.time_since_epoch();
        const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
            duration
        ) % 1000;

        std::ostringstream stream;

        stream
            << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S")
            << "."
            << std::setw(3)
            << std::setfill('0')
            << milliseconds.count()
            << "Z";

        return stream.str();
    }

    std::string format_bool(
        bool value
    )
    {
        return value
            ? "true"
            : "false";
    }

    std::string format_uint64(
        std::uint64_t value
    )
    {
        return std::to_string(value);
    }

    std::string_view to_string(
        ApiMappingStatus status
    ) noexcept
    {
        switch (status)
        {
        case ApiMappingStatus::NotMapped:
            return "NotMapped";
        case ApiMappingStatus::Mapped:
            return "Mapped";
        case ApiMappingStatus::Skipped:
            return "Skipped";
        case ApiMappingStatus::InvalidInput:
            return "InvalidInput";
        case ApiMappingStatus::UnsupportedType:
            return "UnsupportedType";
        case ApiMappingStatus::Failed:
            return "Failed";
        }

        return "Unknown";
    }

    std::string_view to_string(
        ApiTimestampFormat format
    ) noexcept
    {
        switch (format)
        {
        case ApiTimestampFormat::Iso8601Utc:
            return "Iso8601Utc";
        case ApiTimestampFormat::UnixMilliseconds:
            return "UnixMilliseconds";
        case ApiTimestampFormat::EmptyForDefault:
            return "EmptyForDefault";
        }

        return "Unknown";
    }
}