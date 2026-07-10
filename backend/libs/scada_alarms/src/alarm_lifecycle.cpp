#include "scada_alarms/alarm_lifecycle.h"

#include <chrono>
#include <utility>

namespace dispatcher::alarms
{
    namespace
    {
        [[nodiscard]] AlarmTimestamp now()
        {
            return std::chrono::system_clock::now();
        }

        [[nodiscard]] bool is_empty_timestamp(
            AlarmTimestamp timestamp
        ) noexcept
        {
            return timestamp == AlarmTimestamp{};
        }

        [[nodiscard]] AlarmState target_state_for_transition(
            AlarmTransitionType transition_type
        ) noexcept
        {
            switch (transition_type)
            {
            case AlarmTransitionType::Activate:
                return AlarmState::Active;
            case AlarmTransitionType::Acknowledge:
                return AlarmState::Acknowledged;
            case AlarmTransitionType::Clear:
                return AlarmState::Cleared;
            case AlarmTransitionType::Close:
                return AlarmState::Closed;
            case AlarmTransitionType::Shelve:
                return AlarmState::Shelved;
            case AlarmTransitionType::Unshelve:
                return AlarmState::Active;
            case AlarmTransitionType::Suppress:
                return AlarmState::Suppressed;
            case AlarmTransitionType::Unsuppress:
                return AlarmState::Active;
            case AlarmTransitionType::Reactivate:
                return AlarmState::Active;
            }

            return AlarmState::New;
        }

        [[nodiscard]] AlarmTransitionResult make_failure(
            const AlarmRecord& alarm,
            AlarmTransitionValidationCode code,
            std::string message
        )
        {
            AlarmTransitionResult result;

            result.success = false;
            result.code = code;
            result.alarm = alarm;
            result.message = std::move(message);

            return result;
        }
    }

    bool AlarmTransitionRequest::has_actor() const noexcept
    {
        return !actor.empty();
    }

    bool AlarmTransitionRequest::has_reason() const noexcept
    {
        return !reason.empty();
    }

    bool AlarmTransitionRequest::has_timestamp() const noexcept
    {
        return !is_empty_timestamp(timestamp);
    }

    bool AlarmTransitionRecord::has_actor() const noexcept
    {
        return !actor.empty();
    }

    bool AlarmTransitionRecord::has_reason() const noexcept
    {
        return !reason.empty();
    }

    bool AlarmTransitionRecord::has_timestamp() const noexcept
    {
        return !is_empty_timestamp(timestamp);
    }

    bool AlarmTransitionResult::is_success() const noexcept
    {
        return success;
    }

    bool AlarmTransitionResult::has_transition() const noexcept
    {
        return transition.has_value();
    }

    bool AlarmTransitionResult::has_message() const noexcept
    {
        return !message.empty();
    }

    bool can_apply_alarm_transition(
        const AlarmRecord& alarm,
        AlarmTransitionType transition_type
    ) noexcept
    {
        if (alarm.state == AlarmState::Closed)
        {
            return false;
        }

        switch (transition_type)
        {
        case AlarmTransitionType::Activate:
            return alarm.state == AlarmState::New;

        case AlarmTransitionType::Acknowledge:
            return alarm.state == AlarmState::New ||
                alarm.state == AlarmState::Active;

        case AlarmTransitionType::Clear:
            return alarm.state == AlarmState::New ||
                alarm.state == AlarmState::Active ||
                alarm.state == AlarmState::Acknowledged;

        case AlarmTransitionType::Close:
            return alarm.state == AlarmState::Cleared;

        case AlarmTransitionType::Shelve:
            return alarm.state == AlarmState::New ||
                alarm.state == AlarmState::Active ||
                alarm.state == AlarmState::Acknowledged;

        case AlarmTransitionType::Unshelve:
            return alarm.state == AlarmState::Shelved;

        case AlarmTransitionType::Suppress:
            return alarm.state == AlarmState::New ||
                alarm.state == AlarmState::Active ||
                alarm.state == AlarmState::Acknowledged;

        case AlarmTransitionType::Unsuppress:
            return alarm.state == AlarmState::Suppressed;

        case AlarmTransitionType::Reactivate:
            return alarm.state == AlarmState::Cleared;
        }

        return false;
    }

    AlarmTransitionResult apply_alarm_transition(
        const AlarmRecord& alarm,
        const AlarmTransitionRequest& request
    )
    {
        if (alarm.state == AlarmState::Closed)
        {
            return make_failure(
                alarm,
                AlarmTransitionValidationCode::AlarmAlreadyClosed,
                "Alarm is already closed."
            );
        }

        if (request.require_actor && !request.has_actor())
        {
            return make_failure(
                alarm,
                AlarmTransitionValidationCode::MissingActor,
                "Alarm transition requires actor."
            );
        }

        if (request.type == AlarmTransitionType::Acknowledge &&
            !alarm.requires_acknowledgement)
        {
            return make_failure(
                alarm,
                AlarmTransitionValidationCode::AcknowledgementNotRequired,
                "Alarm does not require acknowledgement."
            );
        }

        if (!can_apply_alarm_transition(alarm, request.type))
        {
            return make_failure(
                alarm,
                AlarmTransitionValidationCode::InvalidTransition,
                "Alarm transition is not allowed from current state."
            );
        }

        const auto transition_timestamp = request.has_timestamp()
            ? request.timestamp
            : now();

        AlarmRecord updated_alarm = alarm;

        const auto previous_state = updated_alarm.state;
        const auto new_state = target_state_for_transition(request.type);

        updated_alarm.state = new_state;

        switch (request.type)
        {
        case AlarmTransitionType::Activate:
            updated_alarm.activated_at = transition_timestamp;
            break;

        case AlarmTransitionType::Acknowledge:
            updated_alarm.acknowledged_at = transition_timestamp;
            updated_alarm.acknowledged_by = request.actor;
            break;

        case AlarmTransitionType::Clear:
            updated_alarm.cleared_at = transition_timestamp;
            break;

        case AlarmTransitionType::Close:
            updated_alarm.closed_at = transition_timestamp;
            updated_alarm.closed_by = request.actor;
            break;

        case AlarmTransitionType::Shelve:
            break;

        case AlarmTransitionType::Unshelve:
            updated_alarm.activated_at = transition_timestamp;
            break;

        case AlarmTransitionType::Suppress:
            break;

        case AlarmTransitionType::Unsuppress:
            updated_alarm.activated_at = transition_timestamp;
            break;

        case AlarmTransitionType::Reactivate:
            updated_alarm.activated_at = transition_timestamp;
            ++updated_alarm.occurrence_count;
            break;
        }

        AlarmTransitionRecord transition;

        transition.type = request.type;
        transition.previous_state = previous_state;
        transition.new_state = new_state;
        transition.timestamp = transition_timestamp;
        transition.actor = request.actor;
        transition.reason = request.reason;

        AlarmTransitionResult result;

        result.success = true;
        result.code = AlarmTransitionValidationCode::None;
        result.alarm = updated_alarm;
        result.transition = transition;
        result.message = "Alarm transition applied.";

        return result;
    }

    std::string_view to_string(
        AlarmTransitionType transition_type
    ) noexcept
    {
        switch (transition_type)
        {
        case AlarmTransitionType::Activate:
            return "Activate";
        case AlarmTransitionType::Acknowledge:
            return "Acknowledge";
        case AlarmTransitionType::Clear:
            return "Clear";
        case AlarmTransitionType::Close:
            return "Close";
        case AlarmTransitionType::Shelve:
            return "Shelve";
        case AlarmTransitionType::Unshelve:
            return "Unshelve";
        case AlarmTransitionType::Suppress:
            return "Suppress";
        case AlarmTransitionType::Unsuppress:
            return "Unsuppress";
        case AlarmTransitionType::Reactivate:
            return "Reactivate";
        }

        return "Unknown";
    }

    std::string_view to_string(
        AlarmTransitionValidationCode code
    ) noexcept
    {
        switch (code)
        {
        case AlarmTransitionValidationCode::None:
            return "None";
        case AlarmTransitionValidationCode::InvalidTransition:
            return "InvalidTransition";
        case AlarmTransitionValidationCode::MissingActor:
            return "MissingActor";
        case AlarmTransitionValidationCode::AcknowledgementNotRequired:
            return "AcknowledgementNotRequired";
        case AlarmTransitionValidationCode::AlarmAlreadyClosed:
            return "AlarmAlreadyClosed";
        }

        return "Unknown";
    }
}
