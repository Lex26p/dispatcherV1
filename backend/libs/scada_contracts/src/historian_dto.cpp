#include "scada_contracts/historian/historian_dto.h"

namespace dispatcher::contracts::historian
{
    bool HistorySampleDto::has_sample_id() const noexcept
    {
        return !id.empty();
    }

    bool HistorySampleDto::has_tag_id() const noexcept
    {
        return !tag_id.empty();
    }

    bool HistorySampleDto::has_value() const noexcept
    {
        return !engineering_value.empty();
    }

    bool HistorySampleDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool ArchiveDecisionDto::should_archive_sample() const noexcept
    {
        return should_archive;
    }

    bool ArchiveDecisionDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool HistoryBatchDto::empty() const noexcept
    {
        return samples.empty();
    }

    std::uint64_t HistoryBatchDto::size() const noexcept
    {
        return static_cast<std::uint64_t>(samples.size());
    }

    bool HistoryBatchWriteResultDto::is_success() const noexcept
    {
        return status == "Succeeded";
    }

    bool HistoryBatchWriteResultDto::has_failures() const noexcept
    {
        return failed_count > 0 ||
            status == "Failed" ||
            status == "PartialSuccess";
    }

    bool HistoryBatchWriteResultDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool HistoryQueryTimeRangeDto::has_from() const noexcept
    {
        return !from.empty();
    }

    bool HistoryQueryTimeRangeDto::has_to() const noexcept
    {
        return !to.empty();
    }

    bool HistoryQueryTimeRangeDto::has_range() const noexcept
    {
        return has_from() || has_to();
    }

    bool HistoryQueryFilterDto::has_tag_filter() const noexcept
    {
        return !tag_ids.empty();
    }

    bool HistoryQueryFilterDto::has_quality_filter() const noexcept
    {
        return !quality.empty();
    }

    bool HistoryQueryFilterDto::has_value_type_filter() const noexcept
    {
        return !value_type.empty();
    }

    bool HistoryQueryFilterDto::has_source_filter() const noexcept
    {
        return !source.empty();
    }

    bool HistoryQueryFilterDto::has_any_filter() const noexcept
    {
        return has_tag_filter() ||
            has_quality_filter() ||
            has_value_type_filter() ||
            has_source_filter();
    }

    bool HistoryQueryOptionsDto::has_offset() const noexcept
    {
        return offset > 0;
    }

    bool HistoryQueryDto::has_time_range() const noexcept
    {
        return time_range.has_range();
    }

    bool HistoryQueryDto::has_filters() const noexcept
    {
        return filter.has_any_filter();
    }

    bool HistoryQueryResultDto::empty() const noexcept
    {
        return samples.empty();
    }

    std::uint64_t HistoryQueryResultDto::size() const noexcept
    {
        return static_cast<std::uint64_t>(samples.size());
    }

    bool HistoryQueryResultDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool HistoryValidationIssueDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool HistoryValidationResultDto::is_valid() const noexcept
    {
        return issues.empty();
    }

    bool HistoryValidationResultDto::has_issues() const noexcept
    {
        return !issues.empty();
    }
}
