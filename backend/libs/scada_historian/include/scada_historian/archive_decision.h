#pragma once

#include "scada_historian/history_sample.h"
#include "scada_tags/tag_archive_policy.h"
#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace dispatcher::historian
{
    enum class ArchiveDecisionReason
    {
        None,
        InvalidInput,
        ArchiveDisabled,
        NoValue,
        FirstSample,
        AlwaysPolicy,
        ValueChanged,
        QualityChanged,
        ValueAndQualityChanged,
        DeadbandExceeded,
        DeadbandNotExceeded,
        PeriodicIntervalElapsed,
        PeriodicIntervalNotElapsed,
        AlarmTriggered,
        AlarmNotTriggered,
        Throttled,
        NoChange
    };

    struct ArchiveDecisionOptions
    {
        std::uint32_t periodic_interval_ms = 60000;
        std::uint32_t min_interval_ms = 0;

        double deadband = 0.0;

        bool alarm_triggered = false;
        bool archive_first_sample = true;
    };

    struct ArchiveDecision
    {
        dispatcher::tags::TagId tag_id;

        dispatcher::tags::TagArchivePolicy policy = dispatcher::tags::TagArchivePolicy::Disabled;
        ArchiveDecisionReason reason = ArchiveDecisionReason::None;

        bool should_archive = false;
        bool first_sample = false;
        bool value_changed = false;
        bool quality_changed = false;
        bool throttled = false;

        std::string message;

        [[nodiscard]] bool should_archive_sample() const noexcept;
        [[nodiscard]] bool is_first_sample() const noexcept;
        [[nodiscard]] bool has_value_changed() const noexcept;
        [[nodiscard]] bool has_quality_changed() const noexcept;
        [[nodiscard]] bool is_throttled() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    [[nodiscard]] std::string_view to_string(
        ArchiveDecisionReason reason
    ) noexcept;

    [[nodiscard]] ArchiveDecision decide_archive(
        dispatcher::tags::TagArchivePolicy policy,
        const dispatcher::tags::TagCurrentValue& current_value,
        const std::optional<HistorySample>& previous_sample = std::nullopt,
        ArchiveDecisionOptions options = {}
    );
}
