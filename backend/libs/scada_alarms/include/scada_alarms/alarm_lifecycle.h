#pragma once

#include "scada_alarms/alarm_record.h"

#include <optional>
#include <string>
#include <string_view>

namespace dispatcher::alarms
{
    enum class AlarmTransitionType
    {
        Activate,
        Acknowledge,
        Clear,
        Close,
        Shelve,
        Unshelve,
        Suppress,
        Unsuppress,
        Reactivate
    };

    enum class AlarmTransitionValidationCode
    {
        None,
        InvalidTransition,
        MissingActor,
        AcknowledgementNotRequired,
        AlarmAlreadyClosed
    };

    struct AlarmTransitionRequest
    {
        AlarmTransitionType type = AlarmTransitionType::Activate;

        std::string actor;
        std::string reason;

        AlarmTimestamp timestamp{};

        bool require_actor = false;

        [[nodiscard]] bool has_actor() const noexcept;
        [[nodiscard]] bool has_reason() const noexcept;
        [[nodiscard]] bool has_timestamp() const noexcept;
    };

    struct AlarmTransitionRecord
    {
        AlarmTransitionType type = AlarmTransitionType::Activate;

        AlarmState previous_state = AlarmState::New;
        AlarmState new_state = AlarmState::New;

        AlarmTimestamp timestamp{};

        std::string actor;
        std::string reason;

        [[nodiscard]] bool has_actor() const noexcept;
        [[nodiscard]] bool has_reason() const noexcept;
        [[nodiscard]] bool has_timestamp() const noexcept;
    };

    struct AlarmTransitionResult
    {
        bool success = false;

        AlarmTransitionValidationCode code = AlarmTransitionValidationCode::None;

        AlarmRecord alarm;
        std::optional<AlarmTransitionRecord> transition;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_transition() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    [[nodiscard]] bool can_apply_alarm_transition(
        const AlarmRecord& alarm,
        AlarmTransitionType transition_type
    ) noexcept;

    [[nodiscard]] AlarmTransitionResult apply_alarm_transition(
        const AlarmRecord& alarm,
        const AlarmTransitionRequest& request
    );

    [[nodiscard]] std::string_view to_string(
        AlarmTransitionType transition_type
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        AlarmTransitionValidationCode code
    ) noexcept;
}
