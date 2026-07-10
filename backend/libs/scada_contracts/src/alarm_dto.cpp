#include "scada_contracts/alarms/alarm_dto.h"

namespace dispatcher::contracts::alarms
{
    bool AlarmRecordDto::has_alarm_id() const noexcept
    {
        return !id.empty();
    }

    bool AlarmRecordDto::has_event_id() const noexcept
    {
        return !event_id.empty();
    }

    bool AlarmRecordDto::has_code() const noexcept
    {
        return !code.empty();
    }

    bool AlarmRecordDto::has_title() const noexcept
    {
        return !title.empty();
    }

    bool AlarmRecordDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool AlarmRecordDto::has_source_id() const noexcept
    {
        return !source_id.empty();
    }

    bool AlarmRecordDto::has_tag_id() const noexcept
    {
        return !tag_id.empty();
    }

    bool AlarmRecordDto::has_acknowledged_by() const noexcept
    {
        return !acknowledged_by.empty();
    }

    bool AlarmRecordDto::has_closed_by() const noexcept
    {
        return !closed_by.empty();
    }

    bool AlarmTransitionDto::has_alarm_id() const noexcept
    {
        return !alarm_id.empty();
    }

    bool AlarmTransitionDto::has_actor() const noexcept
    {
        return !actor.empty();
    }

    bool AlarmTransitionDto::has_reason() const noexcept
    {
        return !reason.empty();
    }

    bool AlarmTransitionDto::has_timestamp() const noexcept
    {
        return !timestamp.empty();
    }

    bool AlarmRuleDto::has_rule_id() const noexcept
    {
        return !id.empty();
    }

    bool AlarmRuleDto::has_code() const noexcept
    {
        return !code.empty();
    }

    bool AlarmRuleDto::has_title() const noexcept
    {
        return !title.empty();
    }

    bool AlarmRuleDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool AlarmRuleDto::has_tag_id() const noexcept
    {
        return !tag_id.empty();
    }

    bool AlarmRuleDto::has_expected_quality() const noexcept
    {
        return !expected_quality.empty();
    }

    bool AlarmRuleEvaluationResultDto::is_evaluated() const noexcept
    {
        return evaluated;
    }

    bool AlarmRuleEvaluationResultDto::is_matched() const noexcept
    {
        return matched;
    }

    bool AlarmRuleEvaluationResultDto::has_alarm() const noexcept
    {
        return has_alarm_record;
    }

    bool AlarmRuleEvaluationResultDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool AlarmListDto::empty() const noexcept
    {
        return alarms.empty();
    }

    std::uint64_t AlarmListDto::size() const noexcept
    {
        return static_cast<std::uint64_t>(alarms.size());
    }

    bool AlarmListDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool AlarmValidationIssueDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool AlarmValidationResultDto::is_valid() const noexcept
    {
        return issues.empty();
    }

    bool AlarmValidationResultDto::has_issues() const noexcept
    {
        return !issues.empty();
    }
}
