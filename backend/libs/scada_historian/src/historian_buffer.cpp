#include "scada_historian/historian_buffer.h"

#include <algorithm>
#include <utility>

namespace dispatcher::historian
{
    bool HistorianBufferPushResult::is_accepted() const noexcept
    {
        return accepted;
    }

    bool HistorianBufferPushResult::is_rejected() const noexcept
    {
        return rejected;
    }

    bool HistorianBufferPushResult::has_dropped_oldest() const noexcept
    {
        return dropped_oldest;
    }

    bool HistorianBufferPushResult::has_message() const noexcept
    {
        return !message.empty();
    }

    HistorianBuffer::HistorianBuffer(
        HistorianBufferOptions options
    )
        : options_(options)
    {
        normalize_options();
    }

    const HistorianBufferOptions& HistorianBuffer::options() const noexcept
    {
        return options_;
    }

    bool HistorianBuffer::empty() const noexcept
    {
        return samples_.empty();
    }

    bool HistorianBuffer::is_full() const noexcept
    {
        return samples_.size() >= options_.max_samples;
    }

    bool HistorianBuffer::should_flush() const noexcept
    {
        return samples_.size() >= options_.flush_threshold;
    }

    std::size_t HistorianBuffer::size() const noexcept
    {
        return samples_.size();
    }

    std::size_t HistorianBuffer::capacity() const noexcept
    {
        return options_.max_samples;
    }

    std::size_t HistorianBuffer::flush_threshold() const noexcept
    {
        return options_.flush_threshold;
    }

    HistorianBufferStats HistorianBuffer::stats() const noexcept
    {
        HistorianBufferStats result;

        result.size = samples_.size();
        result.capacity = options_.max_samples;
        result.flush_threshold = options_.flush_threshold;

        result.accepted_count = accepted_count_;
        result.rejected_count = rejected_count_;
        result.dropped_count = dropped_count_;
        result.drained_count = drained_count_;

        result.full = is_full();
        result.flush_recommended = should_flush();

        return result;
    }

    HistorianBufferPushResult HistorianBuffer::push(
        const HistorySample& sample
    )
    {
        return push_internal(sample);
    }

    HistorianBufferPushResult HistorianBuffer::push(
        HistorySample&& sample
    )
    {
        return push_internal(std::move(sample));
    }

    std::size_t HistorianBuffer::push_many(
        const std::vector<HistorySample>& samples
    )
    {
        std::size_t accepted = 0;

        for (const auto& sample : samples)
        {
            const auto result = push(sample);

            if (result.is_accepted())
            {
                ++accepted;
            }
        }

        return accepted;
    }

    HistoryBatch HistorianBuffer::drain()
    {
        return drain_up_to(samples_.size());
    }

    HistoryBatch HistorianBuffer::drain_up_to(
        std::size_t max_samples
    )
    {
        HistoryBatch batch;

        if (max_samples == 0 || samples_.empty())
        {
            return batch;
        }

        const auto count = std::min(
            max_samples,
            samples_.size()
        );

        batch.samples.reserve(count);

        for (std::size_t index = 0; index < count; ++index)
        {
            batch.add(std::move(samples_.front()));
            samples_.pop_front();
            ++drained_count_;
        }

        return batch;
    }

    std::vector<HistorySample> HistorianBuffer::peek_all() const
    {
        return std::vector<HistorySample>(
            samples_.begin(),
            samples_.end()
        );
    }

    void HistorianBuffer::clear() noexcept
    {
        samples_.clear();
    }

    HistorianBufferPushResult HistorianBuffer::push_internal(
        HistorySample sample
    )
    {
        HistorianBufferPushResult result;

        if (!sample.has_valid_identity())
        {
            ++rejected_count_;

            result.accepted = false;
            result.rejected = true;
            result.size_after = samples_.size();
            result.message = "History sample has invalid identity.";

            return result;
        }

        if (is_full())
        {
            if (!options_.drop_oldest_when_full)
            {
                ++rejected_count_;

                result.accepted = false;
                result.rejected = true;
                result.size_after = samples_.size();
                result.message = "Historian buffer is full.";

                return result;
            }

            samples_.pop_front();
            ++dropped_count_;

            result.dropped_oldest = true;
        }

        samples_.push_back(std::move(sample));
        ++accepted_count_;

        result.accepted = true;
        result.rejected = false;
        result.size_after = samples_.size();

        result.message = result.dropped_oldest
            ? "History sample accepted after dropping oldest sample."
            : "History sample accepted.";

        return result;
    }

    void HistorianBuffer::normalize_options() noexcept
    {
        if (options_.max_samples == 0)
        {
            options_.max_samples = 1;
        }

        if (options_.flush_threshold == 0)
        {
            options_.flush_threshold = 1;
        }

        if (options_.flush_threshold > options_.max_samples)
        {
            options_.flush_threshold = options_.max_samples;
        }
    }
}