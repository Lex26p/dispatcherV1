#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace dispatcher::contracts::alarms
{
    struct AlarmRecordDto
    {
        std::string id;
        std::string event_id;

        std::string source_type;
        std::string severity;
        std::string priority;
        std::string state;

        std::string code;
        std::string title;
        std::string message;

        std::string source_id;
        std::string object_id;
        std::string device_id;
        std::string tag_id;

        std::string correlation_id;

        std::string created_at;
        std::string activated_at;
        std::string acknowledged_at;
        std::string cleared_at;
        std::string closed_at;

        std::string acknowledged_by;
        std::string closed_by;

        std::uint64_t occurrence_count = 0;

        bool requires_acknowledgement = true;

        [[nodiscard]] bool has_alarm_id() const noexcept;
        [[nodiscard]] bool has_event_id() const noexcept;
        [[nodiscard]] bool has_code() const noexcept;
        [[nodiscard]] bool has_title() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
        [[nodiscard]] bool has_source_id() const noexcept;
        [[nodiscard]] bool has_tag_id() const noexcept;
        [[nodiscard]] bool has_acknowledged_by() const noexcept;
        [[nodiscard]] bool has_closed_by() const noexcept;
    };

    struct AlarmTransitionDto
    {
        std::string alarm_id;

        std::string type;
        std::string previous_state;
        std::string new_state;

        std::string timestamp;

        std::string actor;
        std::string reason;

        [[nodiscard]] bool has_alarm_id() const noexcept;
        [[nodiscard]] bool has_actor() const noexcept;
        [[nodiscard]] bool has_reason() const noexcept;
        [[nodiscard]] bool has_timestamp() const noexcept;
    };

    struct AlarmRuleDto
    {
        std::string id;

        std::string type;
        std::string state;
        std::string source_type;
        std::string severity;
        std::string priority;
        std::string comparison;

        std::string code;
        std::string title;
        std::string message;

        std::string tag_id;
        std::string object_id;
        std::string device_id;

        double threshold = 0.0;

        std::string expected_quality;

        bool require_good_quality = false;
        bool requires_acknowledgement = true;

        [[nodiscard]] bool has_rule_id() const noexcept;
        [[nodiscard]] bool has_code() const noexcept;
        [[nodiscard]] bool has_title() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
        [[nodiscard]] bool has_tag_id() const noexcept;
        [[nodiscard]] bool has_expected_quality() const noexcept;
    };

    struct AlarmRuleEvaluationResultDto
    {
        std::string status;

        AlarmRuleDto rule;
        AlarmRecordDto alarm;

        bool evaluated = false;
        bool matched = false;
        bool has_alarm_record = false;

        std::string message;

        [[nodiscard]] bool is_evaluated() const noexcept;
        [[nodiscard]] bool is_matched() const noexcept;
        [[nodiscard]] bool has_alarm() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct AlarmListDto
    {
        std::vector<AlarmRecordDto> alarms;

        std::uint64_t total_count = 0;
        bool truncated = false;

        std::string message;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::uint64_t size() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct AlarmValidationIssueDto
    {
        std::string code;
        std::string alarm_id;
        std::string rule_id;
        std::string message;

        [[nodiscard]] bool has_message() const noexcept;
    };

    struct AlarmValidationResultDto
    {
        std::vector<AlarmValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
        [[nodiscard]] bool has_issues() const noexcept;
    };
}
