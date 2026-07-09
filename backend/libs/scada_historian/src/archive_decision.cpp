#include "scada_historian/archive_decision.h"

#include <chrono>
#include <cmath>
#include <optional>
#include <variant>

namespace dispatcher::historian
{
    namespace
    {
        [[nodiscard]] bool is_empty_value(
            const dispatcher::tags::TagValuePayload& value
        ) noexcept
        {
            return std::holds_alternative<std::monostate>(value);
        }

        [[nodiscard]] std::optional<double> to_double(
            const dispatcher::tags::TagValuePayload& value
        )
        {
            if (std::holds_alternative<std::int32_t>(value))
            {
                return static_cast<double>(std::get<std::int32_t>(value));
            }

            if (std::holds_alternative<std::int64_t>(value))
            {
                return static_cast<double>(std::get<std::int64_t>(value));
            }

            if (std::holds_alternative<float>(value))
            {
                return static_cast<double>(std::get<float>(value));
            }

            if (std::holds_alternative<double>(value))
            {
                return std::get<double>(value);
            }

            if (std::holds_alternative<bool>(value))
            {
                return std::get<bool>(value) ? 1.0 : 0.0;
            }

            return std::nullopt;
        }

        [[nodiscard]] bool elapsed_at_least(
            dispatcher::tags::TagTimestamp previous,
            dispatcher::tags::TagTimestamp current,
            std::uint32_t interval_ms
        )
        {
            if (interval_ms == 0)
            {
                return true;
            }

            if (previous == dispatcher::tags::TagTimestamp{} ||
                current == dispatcher::tags::TagTimestamp{})
            {
                return true;
            }

            if (current < previous)
            {
                return true;
            }

            const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                current - previous
            );

            return elapsed.count() >= static_cast<long long>(interval_ms);
        }

        [[nodiscard]] ArchiveDecision make_decision(
            const dispatcher::tags::TagCurrentValue& current_value,
            dispatcher::tags::TagArchivePolicy policy,
            ArchiveDecisionReason reason,
            bool should_archive,
            std::string message
        )
        {
            ArchiveDecision decision;

            decision.tag_id = current_value.tag_id;
            decision.policy = policy;
            decision.reason = reason;
            decision.should_archive = should_archive;
            decision.message = std::move(message);

            return decision;
        }
    }

    bool ArchiveDecision::should_archive_sample() const noexcept
    {
        return should_archive;
    }

    bool ArchiveDecision::is_first_sample() const noexcept
    {
        return first_sample;
    }

    bool ArchiveDecision::has_value_changed() const noexcept
    {
        return value_changed;
    }

    bool ArchiveDecision::has_quality_changed() const noexcept
    {
        return quality_changed;
    }

    bool ArchiveDecision::is_throttled() const noexcept
    {
        return throttled;
    }

    bool ArchiveDecision::has_message() const noexcept
    {
        return !message.empty();
    }

    std::string_view to_string(
        ArchiveDecisionReason reason
    ) noexcept
    {
        switch (reason)
        {
        case ArchiveDecisionReason::None:
            return "None";
        case ArchiveDecisionReason::InvalidInput:
            return "InvalidInput";
        case ArchiveDecisionReason::ArchiveDisabled:
            return "ArchiveDisabled";
        case ArchiveDecisionReason::NoValue:
            return "NoValue";
        case ArchiveDecisionReason::FirstSample:
            return "FirstSample";
        case ArchiveDecisionReason::AlwaysPolicy:
            return "AlwaysPolicy";
        case ArchiveDecisionReason::ValueChanged:
            return "ValueChanged";
        case ArchiveDecisionReason::QualityChanged:
            return "QualityChanged";
        case ArchiveDecisionReason::ValueAndQualityChanged:
            return "ValueAndQualityChanged";
        case ArchiveDecisionReason::DeadbandExceeded:
            return "DeadbandExceeded";
        case ArchiveDecisionReason::DeadbandNotExceeded:
            return "DeadbandNotExceeded";
        case ArchiveDecisionReason::PeriodicIntervalElapsed:
            return "PeriodicIntervalElapsed";
        case ArchiveDecisionReason::PeriodicIntervalNotElapsed:
            return "PeriodicIntervalNotElapsed";
        case ArchiveDecisionReason::AlarmTriggered:
            return "AlarmTriggered";
        case ArchiveDecisionReason::AlarmNotTriggered:
            return "AlarmNotTriggered";
        case ArchiveDecisionReason::Throttled:
            return "Throttled";
        case ArchiveDecisionReason::NoChange:
            return "NoChange";
        }

        return "Unknown";
    }

    ArchiveDecision decide_archive(
        dispatcher::tags::TagArchivePolicy policy,
        const dispatcher::tags::TagCurrentValue& current_value,
        const std::optional<HistorySample>& previous_sample,
        ArchiveDecisionOptions options
    )
    {
        if (current_value.tag_id.empty())
        {
            return make_decision(
                current_value,
                policy,
                ArchiveDecisionReason::InvalidInput,
                false,
                "Current value has empty tag id."
            );
        }

        if (policy == dispatcher::tags::TagArchivePolicy::Disabled)
        {
            return make_decision(
                current_value,
                policy,
                ArchiveDecisionReason::ArchiveDisabled,
                false,
                "Archive policy is disabled."
            );
        }

        if (is_empty_value(current_value.engineering_value))
        {
            return make_decision(
                current_value,
                policy,
                ArchiveDecisionReason::NoValue,
                false,
                "Current value is empty."
            );
        }

        if (!previous_sample.has_value())
        {
            auto decision = make_decision(
                current_value,
                policy,
                ArchiveDecisionReason::FirstSample,
                options.archive_first_sample,
                options.archive_first_sample
                ? "First sample should be archived."
                : "First sample archiving is disabled by options."
            );

            decision.first_sample = true;
            decision.value_changed = true;
            decision.quality_changed = true;

            return decision;
        }

        const auto value_changed =
            previous_sample->engineering_value != current_value.engineering_value;

        const auto quality_changed =
            previous_sample->quality != current_value.quality;

        if (options.min_interval_ms > 0 &&
            !elapsed_at_least(
                previous_sample->timestamp,
                current_value.timestamp,
                options.min_interval_ms
            ))
        {
            auto decision = make_decision(
                current_value,
                policy,
                ArchiveDecisionReason::Throttled,
                false,
                "Archive decision is throttled by minimum interval."
            );

            decision.value_changed = value_changed;
            decision.quality_changed = quality_changed;
            decision.throttled = true;

            return decision;
        }

        switch (policy)
        {
        case dispatcher::tags::TagArchivePolicy::Disabled:
        {
            return make_decision(
                current_value,
                policy,
                ArchiveDecisionReason::ArchiveDisabled,
                false,
                "Archive policy is disabled."
            );
        }

        case dispatcher::tags::TagArchivePolicy::AlwaysButThrottled:
        {
            return make_decision(
                current_value,
                policy,
                ArchiveDecisionReason::AlwaysPolicy,
                true,
                "Archive policy requires archiving each accepted sample."
            );
        }

        case dispatcher::tags::TagArchivePolicy::OnChange:
        {
            auto decision = make_decision(
                current_value,
                policy,
                value_changed
                ? ArchiveDecisionReason::ValueChanged
                : ArchiveDecisionReason::NoChange,
                value_changed,
                value_changed
                ? "Archive policy matched value change."
                : "Archive policy did not match because value did not change."
            );

            decision.value_changed = value_changed;
            decision.quality_changed = quality_changed;

            return decision;
        }

        case dispatcher::tags::TagArchivePolicy::OnQualityChange:
        {
            auto decision = make_decision(
                current_value,
                policy,
                quality_changed
                ? ArchiveDecisionReason::QualityChanged
                : ArchiveDecisionReason::NoChange,
                quality_changed,
                quality_changed
                ? "Archive policy matched quality change."
                : "Archive policy did not match because quality did not change."
            );

            decision.value_changed = value_changed;
            decision.quality_changed = quality_changed;

            return decision;
        }

        case dispatcher::tags::TagArchivePolicy::OnChangeWithDeadband:
        {
            const auto previous_numeric = to_double(previous_sample->engineering_value);
            const auto current_numeric = to_double(current_value.engineering_value);

            if (previous_numeric.has_value() && current_numeric.has_value())
            {
                const auto delta = std::fabs(current_numeric.value() - previous_numeric.value());

                auto decision = make_decision(
                    current_value,
                    policy,
                    delta >= options.deadband
                    ? ArchiveDecisionReason::DeadbandExceeded
                    : ArchiveDecisionReason::DeadbandNotExceeded,
                    delta >= options.deadband,
                    delta >= options.deadband
                    ? "Archive policy matched because deadband was exceeded."
                    : "Archive policy did not match because deadband was not exceeded."
                );

                decision.value_changed = value_changed;
                decision.quality_changed = quality_changed;

                return decision;
            }

            auto decision = make_decision(
                current_value,
                policy,
                value_changed
                ? ArchiveDecisionReason::ValueChanged
                : ArchiveDecisionReason::NoChange,
                value_changed,
                value_changed
                ? "Archive policy matched non-numeric value change."
                : "Archive policy did not match because non-numeric value did not change."
            );

            decision.value_changed = value_changed;
            decision.quality_changed = quality_changed;

            return decision;
        }

        case dispatcher::tags::TagArchivePolicy::Periodic:
        {
            const auto elapsed = elapsed_at_least(
                previous_sample->timestamp,
                current_value.timestamp,
                options.periodic_interval_ms
            );

            auto decision = make_decision(
                current_value,
                policy,
                elapsed
                ? ArchiveDecisionReason::PeriodicIntervalElapsed
                : ArchiveDecisionReason::PeriodicIntervalNotElapsed,
                elapsed,
                elapsed
                ? "Archive policy matched periodic interval."
                : "Archive policy did not match because periodic interval did not elapse."
            );

            decision.value_changed = value_changed;
            decision.quality_changed = quality_changed;

            return decision;
        }

        case dispatcher::tags::TagArchivePolicy::OnAlarm:
        {
            auto decision = make_decision(
                current_value,
                policy,
                options.alarm_triggered
                ? ArchiveDecisionReason::AlarmTriggered
                : ArchiveDecisionReason::AlarmNotTriggered,
                options.alarm_triggered,
                options.alarm_triggered
                ? "Archive policy matched alarm trigger."
                : "Archive policy did not match because alarm was not triggered."
            );

            decision.value_changed = value_changed;
            decision.quality_changed = quality_changed;

            return decision;
        }
        }

        return make_decision(
            current_value,
            policy,
            ArchiveDecisionReason::InvalidInput,
            false,
            "Unsupported archive policy."
        );
    }
}