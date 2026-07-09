#pragma once

#include "scada_alarms/alarm_ids.h"
#include "scada_events/event_ids.h"
#include "scada_events/event_record.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace dispatcher::alarms
{
    using AlarmTimestamp = dispatcher::events::EventTimestamp;

    enum class AlarmSourceType
    {
        Unknown,
        System,
        Object,
        Device,
        Tag,
        Communication,
        Runtime,
        Historian,
        Polling,
        Protocol,
        Script,
        Integration,
        Manual
    };

    enum class AlarmSeverity
    {
        Information,
        Warning,
        Minor,
        Major,
        Critical
    };

    enum class AlarmPriority
    {
        Low,
        Medium,
        High,
        Critical
    };

    enum class AlarmState
    {
        New,
        Active,
        Acknowledged,
        Cleared,
        Closed,
        Shelved,
        Suppressed
    };

    struct AlarmRecord
    {
        AlarmId id;
        dispatcher::events::EventId event_id;

        AlarmSourceType source_type = AlarmSourceType::Unknown;
        AlarmSeverity severity = AlarmSeverity::Warning;
        AlarmPriority priority = AlarmPriority::Medium;
        AlarmState state = AlarmState::New;

        std::string code;
        std::string title;
        std::string message;

        std::string source_id;
        std::string object_id;
        std::string device_id;
        std::string tag_id;

        std::string correlation_id;

        AlarmTimestamp created_at{};
        AlarmTimestamp activated_at{};
        AlarmTimestamp acknowledged_at{};
        AlarmTimestamp cleared_at{};
        AlarmTimestamp closed_at{};

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
        [[nodiscard]] bool has_object_id() const noexcept;
        [[nodiscard]] bool has_device_id() const noexcept;
        [[nodiscard]] bool has_tag_id() const noexcept;
        [[nodiscard]] bool has_correlation_id() const noexcept;
        [[nodiscard]] bool has_acknowledged_by() const noexcept;
        [[nodiscard]] bool has_closed_by() const noexcept;
        [[nodiscard]] bool is_active() const noexcept;
        [[nodiscard]] bool is_closed() const noexcept;
        [[nodiscard]] bool has_valid_identity() const noexcept;
    };

    [[nodiscard]] AlarmRecord make_alarm_record(
        AlarmSourceType source_type,
        AlarmSeverity severity,
        AlarmPriority priority,
        std::string code,
        std::string title,
        std::string message
    );

    [[nodiscard]] dispatcher::events::EventSeverity to_event_severity(
        AlarmSeverity severity
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        AlarmSourceType source_type
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        AlarmSeverity severity
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        AlarmPriority priority
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        AlarmState state
    ) noexcept;
}