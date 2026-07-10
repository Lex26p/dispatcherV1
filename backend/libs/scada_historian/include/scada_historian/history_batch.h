#pragma once

#include "scada_historian/history_sample.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::historian
{
    enum class HistoryBatchWriteStatus
    {
        NotStarted,
        Succeeded,
        PartialSuccess,
        Failed,
        Skipped
    };

    struct HistoryBatch
    {
        std::vector<HistorySample> samples;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;

        void add(
            const HistorySample& sample
        );

        void add(
            HistorySample&& sample
        );

        void clear() noexcept;
    };

    struct HistoryBatchWriteResult
    {
        HistoryBatchWriteStatus status = HistoryBatchWriteStatus::NotStarted;

        std::size_t attempted_count = 0;
        std::size_t written_count = 0;
        std::size_t failed_count = 0;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_failures() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    class IHistoryBatchWriter
    {
    public:
        virtual ~IHistoryBatchWriter() = default;

        [[nodiscard]] virtual HistoryBatchWriteResult write_batch(
            const HistoryBatch& batch
        ) = 0;
    };

    [[nodiscard]] std::string_view to_string(
        HistoryBatchWriteStatus status
    ) noexcept;
}
