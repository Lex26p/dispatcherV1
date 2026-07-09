#include "scada_alarms/alarm_rule.h"

#include <cmath>
#include <cstdint>
#include <optional>
#include <utility>
#include <variant>

namespace dispatcher::alarms
{
    namespace
    {
        [[nodiscard]] bool is_good_quality(
            dispatcher::tags::TagQuality quality
        ) noexcept
        {
            return quality == dispatcher::tags::TagQuality::Good ||
                quality == dispatcher::tags::TagQuality::Manual ||
                quality == dispatcher::tags::TagQuality::Simulation;
        }

        [[nodiscard]] bool is_bad_quality(
            dispatcher::tags::TagQuality quality
        ) noexcept
        {
            return quality == dispatcher::tags::TagQuality::Bad ||
                quality == dispatcher::tags::TagQuality::CommunicationError ||
                quality == dispatcher::tags::TagQuality::Timeout ||
                quality == dispatcher::tags::TagQuality::DeviceError ||
                quality == dispatcher::tags::TagQuality::ProtocolError ||
                quality == dispatcher::tags::TagQuality::OutOfRange ||
                quality == dispatcher::tags::TagQuality::Stale ||
                quality == dispatcher::tags::TagQuality::Disabled ||
                quality == dispatcher::tags::TagQuality::NotInitialized;
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

        [[nodiscard]] bool compare_numeric(
            double actual,
            AlarmRuleComparison comparison,
            double threshold
        ) noexcept
        {
            constexpr double epsilon = 0.000000001;

            switch (comparison)
            {
            case AlarmRuleComparison::GreaterThan:
                return actual > threshold;
            case AlarmRuleComparison::GreaterOrEqual:
                return actual >= threshold;
            case AlarmRuleComparison::LessThan:
                return actual < threshold;
            case AlarmRuleComparison::LessOrEqual:
                return actual <= threshold;
            case AlarmRuleComparison::Equal:
                return std::fabs(actual - threshold) <= epsilon;
            case AlarmRuleComparison::NotEqual:
                return std::fabs(actual - threshold) > epsilon;
            }

            return false;
        }

        [[nodiscard]] AlarmRuleEvaluationResult make_result(
            const AlarmRule& rule,
            AlarmRuleEvaluationStatus status,
            bool evaluated,
            bool matched,
            std::string message
        )
        {
            AlarmRuleEvaluationResult result;

            result.status = status;
            result.rule = rule;
            result.evaluated = evaluated;
            result.matched = matched;
            result.message = std::move(message);

            return result;
        }

        [[nodiscard]] AlarmRecord make_alarm_from_rule(
            const AlarmRule& rule
        )
        {
            auto alarm = make_alarm_record(
                rule.source_type,
                rule.severity,
                rule.priority,
                rule.code,
                rule.title,
                rule.message
            );

            alarm.tag_id = rule.tag_id;
            alarm.object_id = rule.object_id;
            alarm.device_id = rule.device_id;
            alarm.source_id = rule.has_tag_id()
                ? rule.tag_id
                : rule.device_id;

            alarm.requires_acknowledgement = rule.requires_acknowledgement;

            return alarm;
        }
    }

    bool AlarmRule::has_rule_id() const noexcept
    {
        return !id.empty();
    }

    bool AlarmRule::has_code() const noexcept
    {
        return !code.empty();
    }

    bool AlarmRule::has_title() const noexcept
    {
        return !title.empty();
    }

    bool AlarmRule::has_message() const noexcept
    {
        return !message.empty();
    }

    bool AlarmRule::has_tag_id() const noexcept
    {
        return !tag_id.empty();
    }

    bool AlarmRule::has_object_id() const noexcept
    {
        return !object_id.empty();
    }

    bool AlarmRule::has_device_id() const noexcept
    {
        return !device_id.empty();
    }

    bool AlarmRule::is_enabled() const noexcept
    {
        return state == AlarmRuleState::Enabled;
    }

    bool AlarmRule::has_expected_quality() const noexcept
    {
        return expected_quality.has_value();
    }

    bool AlarmRule::has_valid_identity() const noexcept
    {
        return has_code() && has_title();
    }

    bool AlarmRuleEvaluationResult::is_evaluated() const noexcept
    {
        return evaluated;
    }

    bool AlarmRuleEvaluationResult::is_matched() const noexcept
    {
        return matched;
    }

    bool AlarmRuleEvaluationResult::has_alarm() const noexcept
    {
        return alarm.has_value();
    }

    bool AlarmRuleEvaluationResult::has_message() const noexcept
    {
        return !message.empty();
    }

    AlarmRuleEvaluationResult evaluate_alarm_rule(
        const AlarmRule& rule,
        const dispatcher::tags::TagCurrentValue& current_value
    )
    {
        if (!rule.is_enabled())
        {
            return make_result(
                rule,
                AlarmRuleEvaluationStatus::DisabledRule,
                false,
                false,
                "Alarm rule is disabled."
            );
        }

        if (!rule.has_valid_identity())
        {
            return make_result(
                rule,
                AlarmRuleEvaluationStatus::InvalidRule,
                false,
                false,
                "Alarm rule has invalid identity."
            );
        }

        if (rule.require_good_quality &&
            !is_good_quality(current_value.quality))
        {
            return make_result(
                rule,
                AlarmRuleEvaluationStatus::NotMatched,
                true,
                false,
                "Alarm rule requires good quality."
            );
        }

        bool matched = false;

        switch (rule.type)
        {
        case AlarmRuleType::NumericThreshold:
        {
            const auto numeric_value = to_double(current_value.engineering_value);

            if (!numeric_value.has_value())
            {
                return make_result(
                    rule,
                    AlarmRuleEvaluationStatus::InvalidValue,
                    true,
                    false,
                    "Alarm rule expected numeric engineering value."
                );
            }

            matched = compare_numeric(
                numeric_value.value(),
                rule.comparison,
                rule.threshold
            );

            break;
        }

        case AlarmRuleType::QualityEquals:
        {
            if (!rule.expected_quality.has_value())
            {
                return make_result(
                    rule,
                    AlarmRuleEvaluationStatus::InvalidRule,
                    true,
                    false,
                    "Alarm rule expected quality is not set."
                );
            }

            matched = current_value.quality == rule.expected_quality.value();
            break;
        }

        case AlarmRuleType::QualityNotGood:
        {
            matched = !is_good_quality(current_value.quality);
            break;
        }

        case AlarmRuleType::QualityBad:
        {
            matched = is_bad_quality(current_value.quality);
            break;
        }
        }

        auto result = make_result(
            rule,
            matched
            ? AlarmRuleEvaluationStatus::Matched
            : AlarmRuleEvaluationStatus::NotMatched,
            true,
            matched,
            matched
            ? "Alarm rule matched."
            : "Alarm rule did not match."
        );

        if (matched)
        {
            result.alarm = make_alarm_from_rule(rule);
        }

        return result;
    }

    std::string_view to_string(
        AlarmRuleType type
    ) noexcept
    {
        switch (type)
        {
        case AlarmRuleType::NumericThreshold:
            return "NumericThreshold";
        case AlarmRuleType::QualityEquals:
            return "QualityEquals";
        case AlarmRuleType::QualityNotGood:
            return "QualityNotGood";
        case AlarmRuleType::QualityBad:
            return "QualityBad";
        }

        return "Unknown";
    }

    std::string_view to_string(
        AlarmRuleState state
    ) noexcept
    {
        switch (state)
        {
        case AlarmRuleState::Draft:
            return "Draft";
        case AlarmRuleState::Enabled:
            return "Enabled";
        case AlarmRuleState::Disabled:
            return "Disabled";
        }

        return "Unknown";
    }

    std::string_view to_string(
        AlarmRuleComparison comparison
    ) noexcept
    {
        switch (comparison)
        {
        case AlarmRuleComparison::GreaterThan:
            return "GreaterThan";
        case AlarmRuleComparison::GreaterOrEqual:
            return "GreaterOrEqual";
        case AlarmRuleComparison::LessThan:
            return "LessThan";
        case AlarmRuleComparison::LessOrEqual:
            return "LessOrEqual";
        case AlarmRuleComparison::Equal:
            return "Equal";
        case AlarmRuleComparison::NotEqual:
            return "NotEqual";
        }

        return "Unknown";
    }

    std::string_view to_string(
        AlarmRuleEvaluationStatus status
    ) noexcept
    {
        switch (status)
        {
        case AlarmRuleEvaluationStatus::NotEvaluated:
            return "NotEvaluated";
        case AlarmRuleEvaluationStatus::Matched:
            return "Matched";
        case AlarmRuleEvaluationStatus::NotMatched:
            return "NotMatched";
        case AlarmRuleEvaluationStatus::DisabledRule:
            return "DisabledRule";
        case AlarmRuleEvaluationStatus::InvalidRule:
            return "InvalidRule";
        case AlarmRuleEvaluationStatus::InvalidValue:
            return "InvalidValue";
        }

        return "Unknown";
    }
}