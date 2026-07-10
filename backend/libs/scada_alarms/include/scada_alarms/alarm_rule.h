#pragma once

#include "scada_alarms/alarm_record.h"
#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_quality.h"

#include <optional>
#include <string>
#include <string_view>

namespace dispatcher::alarms
{
    enum class AlarmRuleType
    {
        NumericThreshold,
        QualityEquals,
        QualityNotGood,
        QualityBad
    };

    enum class AlarmRuleState
    {
        Draft,
        Enabled,
        Disabled
    };

    enum class AlarmRuleComparison
    {
        GreaterThan,
        GreaterOrEqual,
        LessThan,
        LessOrEqual,
        Equal,
        NotEqual
    };

    enum class AlarmRuleEvaluationStatus
    {
        NotEvaluated,
        Matched,
        NotMatched,
        DisabledRule,
        InvalidRule,
        InvalidValue
    };

    struct AlarmRule
    {
        AlarmRuleId id;

        AlarmRuleType type = AlarmRuleType::NumericThreshold;
        AlarmRuleState state = AlarmRuleState::Draft;

        AlarmSourceType source_type = AlarmSourceType::Tag;
        AlarmSeverity severity = AlarmSeverity::Warning;
        AlarmPriority priority = AlarmPriority::Medium;

        AlarmRuleComparison comparison = AlarmRuleComparison::GreaterOrEqual;

        std::string code;
        std::string title;
        std::string message;

        std::string tag_id;
        std::string object_id;
        std::string device_id;

        double threshold = 0.0;

        std::optional<dispatcher::tags::TagQuality> expected_quality;

        bool require_good_quality = false;
        bool requires_acknowledgement = true;

        [[nodiscard]] bool has_rule_id() const noexcept;
        [[nodiscard]] bool has_code() const noexcept;
        [[nodiscard]] bool has_title() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
        [[nodiscard]] bool has_tag_id() const noexcept;
        [[nodiscard]] bool has_object_id() const noexcept;
        [[nodiscard]] bool has_device_id() const noexcept;
        [[nodiscard]] bool is_enabled() const noexcept;
        [[nodiscard]] bool has_expected_quality() const noexcept;
        [[nodiscard]] bool has_valid_identity() const noexcept;
    };

    struct AlarmRuleEvaluationResult
    {
        AlarmRuleEvaluationStatus status = AlarmRuleEvaluationStatus::NotEvaluated;

        AlarmRule rule;
        std::optional<AlarmRecord> alarm;

        bool evaluated = false;
        bool matched = false;

        std::string message;

        [[nodiscard]] bool is_evaluated() const noexcept;
        [[nodiscard]] bool is_matched() const noexcept;
        [[nodiscard]] bool has_alarm() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    [[nodiscard]] AlarmRuleEvaluationResult evaluate_alarm_rule(
        const AlarmRule& rule,
        const dispatcher::tags::TagCurrentValue& current_value
    );

    [[nodiscard]] std::string_view to_string(
        AlarmRuleType type
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        AlarmRuleState state
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        AlarmRuleComparison comparison
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        AlarmRuleEvaluationStatus status
    ) noexcept;
}
