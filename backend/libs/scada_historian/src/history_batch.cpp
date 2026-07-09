#include "scada_historian/history_batch.h"

#include <utility>

namespace dispatcher::historian
{
    bool HistoryBatch::empty() const noexcept
    {
        return samples.empty();
    }

    std::size_t HistoryBatch::size() const noexcept
    {
        return samples.size();
    }

    void HistoryBatch::add(
        const HistorySample& sample
    )
    {
        samples.push_back(sample);
    }

    void HistoryBatch::add(
        HistorySample&& sample
    )
    {
        samples.push_back(std::move(sample));
    }

    void HistoryBatch::clear() noexcept
    {
        samples.clear();
    }

    bool HistoryBatchWriteResult::is_success() const noexcept
    {
        return status == HistoryBatchWriteStatus::Succeeded;
    }

    bool HistoryBatchWriteResult::has_failures() const noexcept
    {
        return failed_count > 0 ||
            status == HistoryBatchWriteStatus::Failed ||
            status == HistoryBatchWriteStatus::PartialSuccess;
    }

    bool HistoryBatchWriteResult::has_message() const noexcept
    {
        return !message.empty();
    }

    std::string_view to_string(
        HistoryBatchWriteStatus status
    ) noexcept
    {
        switch (status)
        {
        case HistoryBatchWriteStatus::NotStarted:
            return "NotStarted";
        case HistoryBatchWriteStatus::Succeeded:
            return "Succeeded";
        case HistoryBatchWriteStatus::PartialSuccess:
            return "PartialSuccess";
        case HistoryBatchWriteStatus::Failed:
            return "Failed";
        case HistoryBatchWriteStatus::Skipped:
            return "Skipped";
        }

        return "Unknown";
    }
}