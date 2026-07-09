#pragma once

#include "scada_historian/history_batch.h"
#include "scada_historian/history_sample.h"

#include <cstddef>
#include <cstdint>
#include <deque>
#include <string>
#include <vector>

namespace dispatcher::historian
{
    struct HistorianBufferOptions
    {
        std::size_t max_samples = 10000;
        std::size_t flush_threshold = 1000;

        bool drop_oldest_when_full = false;
    };

    struct HistorianBufferStats
    {
        std::size_t size = 0;
        std::size_t capacity = 0;
        std::size_t flush_threshold = 0;

        std::uint64_t accepted_count = 0;
        std::uint64_t rejected_count = 0;
        std::uint64_t dropped_count = 0;
        std::uint64_t drained_count = 0;

        bool full = false;
        bool flush_recommended = false;
    };

    struct HistorianBufferPushResult
    {
        bool accepted = false;
        bool rejected = false;
        bool dropped_oldest = false;

        std::size_t size_after = 0;

        std::string message;

        [[nodiscard]] bool is_accepted() const noexcept;
        [[nodiscard]] bool is_rejected() const noexcept;
        [[nodiscard]] bool has_dropped_oldest() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    class HistorianBuffer
    {
    public:
        explicit HistorianBuffer(
            HistorianBufferOptions options = {}
        );

        [[nodiscard]] const HistorianBufferOptions& options() const noexcept;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] bool is_full() const noexcept;
        [[nodiscard]] bool should_flush() const noexcept;

        [[nodiscard]] std::size_t size() const noexcept;
        [[nodiscard]] std::size_t capacity() const noexcept;
        [[nodiscard]] std::size_t flush_threshold() const noexcept;

        [[nodiscard]] HistorianBufferStats stats() const noexcept;

        [[nodiscard]] HistorianBufferPushResult push(
            const HistorySample& sample
        );

        [[nodiscard]] HistorianBufferPushResult push(
            HistorySample&& sample
        );

        [[nodiscard]] std::size_t push_many(
            const std::vector<HistorySample>& samples
        );

        [[nodiscard]] HistoryBatch drain();

        [[nodiscard]] HistoryBatch drain_up_to(
            std::size_t max_samples
        );

        [[nodiscard]] std::vector<HistorySample> peek_all() const;

        void clear() noexcept;

    private:
        HistorianBufferOptions options_;

        std::deque<HistorySample> samples_;

        std::uint64_t accepted_count_ = 0;
        std::uint64_t rejected_count_ = 0;
        std::uint64_t dropped_count_ = 0;
        std::uint64_t drained_count_ = 0;

        [[nodiscard]] HistorianBufferPushResult push_internal(
            HistorySample sample
        );

        void normalize_options() noexcept;
    };
}