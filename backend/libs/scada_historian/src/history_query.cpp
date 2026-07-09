#include "scada_historian/history_query.h"

namespace dispatcher::historian
{
    namespace
    {
        constexpr std::size_t max_tag_filter_count = 1000;

        void add_issue(
            HistoryQueryValidationResult& result,
            HistoryQueryValidationCode code,
            std::string message
        )
        {
            result.issues.push_back(
                HistoryQueryValidationIssue{
                    .code = code,
                    .message = std::move(message)
                }
            );
        }
    }

    bool HistoryQueryTimeRange::has_from() const noexcept
    {
        return from.has_value();
    }

    bool HistoryQueryTimeRange::has_to() const noexcept
    {
        return to.has_value();
    }

    bool HistoryQueryTimeRange::has_range() const noexcept
    {
        return has_from() || has_to();
    }

    bool HistoryQueryTimeRange::is_valid() const noexcept
    {
        if (!from.has_value() || !to.has_value())
        {
            return true;
        }

        return from.value() <= to.value();
    }

    bool HistoryQueryFilter::has_tag_filter() const noexcept
    {
        return !tag_ids.empty();
    }

    bool HistoryQueryFilter::has_quality_filter() const noexcept
    {
        return quality.has_value();
    }

    bool HistoryQueryFilter::has_value_type_filter() const noexcept
    {
        return value_type.has_value();
    }

    bool HistoryQueryFilter::has_source_filter() const noexcept
    {
        return source.has_value();
    }

    bool HistoryQueryFilter::has_any_filter() const noexcept
    {
        return has_tag_filter() ||
            has_quality_filter() ||
            has_value_type_filter() ||
            has_source_filter();
    }

    std::uint32_t HistoryQueryOptions::normalized_limit() const noexcept
    {
        const auto effective_max_limit = max_limit == 0
            ? 1U
            : max_limit;

        if (limit == 0)
        {
            return 1U;
        }

        return limit > effective_max_limit
            ? effective_max_limit
            : limit;
    }

    bool HistoryQueryOptions::has_offset() const noexcept
    {
        return offset > 0;
    }

    bool HistoryQuery::has_time_range() const noexcept
    {
        return time_range.has_range();
    }

    bool HistoryQuery::has_filters() const noexcept
    {
        return filter.has_any_filter();
    }

    bool HistoryQuery::is_valid() const noexcept
    {
        return time_range.is_valid() &&
            options.limit > 0 &&
            options.max_limit > 0 &&
            filter.tag_ids.size() <= max_tag_filter_count;
    }

    bool HistoryQueryValidationIssue::has_message() const noexcept
    {
        return !message.empty();
    }

    bool HistoryQueryValidationResult::is_valid() const noexcept
    {
        return issues.empty();
    }

    bool HistoryQueryValidationResult::has_issues() const noexcept
    {
        return !issues.empty();
    }

    bool HistoryQueryResult::empty() const noexcept
    {
        return samples.empty();
    }

    std::size_t HistoryQueryResult::size() const noexcept
    {
        return samples.size();
    }

    bool HistoryQueryResult::has_message() const noexcept
    {
        return !message.empty();
    }

    std::string_view to_string(
        HistoryQueryOrder order
    ) noexcept
    {
        switch (order)
        {
        case HistoryQueryOrder::Ascending:
            return "Ascending";
        case HistoryQueryOrder::Descending:
            return "Descending";
        }

        return "Unknown";
    }

    std::string_view to_string(
        HistoryQueryValidationCode code
    ) noexcept
    {
        switch (code)
        {
        case HistoryQueryValidationCode::None:
            return "None";
        case HistoryQueryValidationCode::InvalidTimeRange:
            return "InvalidTimeRange";
        case HistoryQueryValidationCode::InvalidLimit:
            return "InvalidLimit";
        case HistoryQueryValidationCode::InvalidMaxLimit:
            return "InvalidMaxLimit";
        case HistoryQueryValidationCode::TooManyTags:
            return "TooManyTags";
        }

        return "Unknown";
    }

    HistoryQueryValidationResult validate_history_query(
        const HistoryQuery& query
    )
    {
        HistoryQueryValidationResult result;

        if (!query.time_range.is_valid())
        {
            add_issue(
                result,
                HistoryQueryValidationCode::InvalidTimeRange,
                "History query time range is invalid."
            );
        }

        if (query.options.limit == 0)
        {
            add_issue(
                result,
                HistoryQueryValidationCode::InvalidLimit,
                "History query limit must be greater than zero."
            );
        }

        if (query.options.max_limit == 0)
        {
            add_issue(
                result,
                HistoryQueryValidationCode::InvalidMaxLimit,
                "History query max limit must be greater than zero."
            );
        }

        if (query.filter.tag_ids.size() > max_tag_filter_count)
        {
            add_issue(
                result,
                HistoryQueryValidationCode::TooManyTags,
                "History query contains too many tag ids."
            );
        }

        return result;
    }
}