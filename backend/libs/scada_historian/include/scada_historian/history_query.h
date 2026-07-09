#pragma once

#include "scada_historian/history_sample.h"
#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"
#include "scada_tags/tag_quality.h"
#include "scada_tags/tag_value_type.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::historian
{
    enum class HistoryQueryOrder
    {
        Ascending,
        Descending
    };

    enum class HistoryQueryValidationCode
    {
        None,
        InvalidTimeRange,
        InvalidLimit,
        InvalidMaxLimit,
        TooManyTags
    };

    struct HistoryQueryTimeRange
    {
        std::optional<dispatcher::tags::TagTimestamp> from;
        std::optional<dispatcher::tags::TagTimestamp> to;

        [[nodiscard]] bool has_from() const noexcept;
        [[nodiscard]] bool has_to() const noexcept;
        [[nodiscard]] bool has_range() const noexcept;
        [[nodiscard]] bool is_valid() const noexcept;
    };

    struct HistoryQueryFilter
    {
        std::vector<dispatcher::tags::TagId> tag_ids;

        std::optional<dispatcher::tags::TagQuality> quality;
        std::optional<dispatcher::tags::TagValueType> value_type;
        std::optional<dispatcher::tags::TagValueSource> source;

        [[nodiscard]] bool has_tag_filter() const noexcept;
        [[nodiscard]] bool has_quality_filter() const noexcept;
        [[nodiscard]] bool has_value_type_filter() const noexcept;
        [[nodiscard]] bool has_source_filter() const noexcept;
        [[nodiscard]] bool has_any_filter() const noexcept;
    };

    struct HistoryQueryOptions
    {
        HistoryQueryOrder order = HistoryQueryOrder::Ascending;

        std::uint32_t limit = 1000;
        std::uint32_t offset = 0;
        std::uint32_t max_limit = 10000;

        bool include_raw_value = true;
        bool include_message = true;

        [[nodiscard]] std::uint32_t normalized_limit() const noexcept;
        [[nodiscard]] bool has_offset() const noexcept;
    };

    struct HistoryQuery
    {
        HistoryQueryTimeRange time_range;
        HistoryQueryFilter filter;
        HistoryQueryOptions options;

        [[nodiscard]] bool has_time_range() const noexcept;
        [[nodiscard]] bool has_filters() const noexcept;
        [[nodiscard]] bool is_valid() const noexcept;
    };

    struct HistoryQueryValidationIssue
    {
        HistoryQueryValidationCode code = HistoryQueryValidationCode::None;
        std::string message;

        [[nodiscard]] bool has_message() const noexcept;
    };

    struct HistoryQueryValidationResult
    {
        std::vector<HistoryQueryValidationIssue> issues;

        [[nodiscard]] bool is_valid() const noexcept;
        [[nodiscard]] bool has_issues() const noexcept;
    };

    struct HistoryQueryResult
    {
        std::vector<HistorySample> samples;

        std::uint64_t total_count = 0;
        bool truncated = false;

        std::string message;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    [[nodiscard]] std::string_view to_string(
        HistoryQueryOrder order
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        HistoryQueryValidationCode code
    ) noexcept;

    [[nodiscard]] HistoryQueryValidationResult validate_history_query(
        const HistoryQuery& query
    );
}