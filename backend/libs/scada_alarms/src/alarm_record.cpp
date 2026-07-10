#include "scada_alarms/alarm_record.h"

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
    }

    bool AlarmRecord::has_alarm_id() const noexcept
    {
        return !id.empty();
    }

    bool AlarmRecord::has_event_id() const noexcept
    {
        return !event_id.empty();
    }

    bool AlarmRecord::has_code() const noexcept
    {
        return !code.empty();
    }

    bool AlarmRecord::has_title() const noexcept
    {
        return !title.empty();
    }

    bool AlarmRecord::has_message() const noexcept
    {
        return !message.empty();
    }

    bool AlarmRecord::has_source_id() const noexcept
    {
        return !source_id.empty();
    }

    bool AlarmRecord::has_object_id() const noexcept
    {
        return !object_id.empty();
    }

    bool AlarmRecord::has_device_id() const noexcept
    {
        return !device_id.empty();
    }

    bool AlarmRecord::has_tag_id() const noexcept
    {
        return !tag_id.empty();
    }

    bool AlarmRecord::has_correlation_id() const noexcept
    {
        return !correlation_id.empty();
    }

    bool AlarmRecord::has_acknowledged_by() const noexcept
    {
        return !acknowledged_by.empty();
    }

    bool AlarmRecord::has_closed_by() const noexcept
    {
        return !closed_by.empty();
    }

    bool AlarmRecord::is_active() const noexcept
    {
        return state == AlarmState::New ||
            state == AlarmState::Active ||
            state == AlarmState::Acknowledged;
    }

    bool AlarmRecord::is_closed() const noexcept
    {
        return state == AlarmState::Closed;
    }

    bool AlarmRecord::has_valid_identity() const noexcept
    {
        return has_code() && has_title();
    }

    AlarmRecord make_alarm_record(
        AlarmSourceType source_type,
        AlarmSeverity severity,
        AlarmPriority priority,
        std::string code,
        std::string title,
        std::string message
    )
    {
        AlarmRecord alarm;

        alarm.source_type = source_type;
        alarm.severity = severity;
        alarm.priority = priority;
        alarm.state = AlarmState::New;
        alarm.code = std::move(code);
        alarm.title = std::move(title);
        alarm.message = std::move(message);
        alarm.created_at = now();
        alarm.activated_at = alarm.created_at;
        alarm.occurrence_count = 1;

        return alarm;
    }

    dispatcher::events::EventSeverity to_event_severity(
        AlarmSeverity severity
    ) noexcept
    {
        switch (severity)
        {
        case AlarmSeverity::Information:
            return dispatcher::events::EventSeverity::Information;
        case AlarmSeverity::Warning:
            return dispatcher::events::EventSeverity::Warning;
        case AlarmSeverity::Minor:
            return dispatcher::events::EventSeverity::Warning;
        case AlarmSeverity::Major:
            return dispatcher::events::EventSeverity::Error;
        case AlarmSeverity::Critical:
            return dispatcher::events::EventSeverity::Critical;
        }

        return dispatcher::events::EventSeverity::Warning;
    }

    std::string_view to_string(
        AlarmSourceType source_type
    ) noexcept
    {
        switch (source_type)
        {
        case AlarmSourceType::Unknown:
            return "Unknown";
        case AlarmSourceType::System:
            return "System";
        case AlarmSourceType::Object:
            return "Object";
        case AlarmSourceType::Device:
            return "Device";
        case AlarmSourceType::Tag:
            return "Tag";
        case AlarmSourceType::Communication:
            return "Communication";
        case AlarmSourceType::Runtime:
            return "Runtime";
        case AlarmSourceType::Historian:
            return "Historian";
        case AlarmSourceType::Polling:
            return "Polling";
        case AlarmSourceType::Protocol:
            return "Protocol";
        case AlarmSourceType::Script:
            return "Script";
        case AlarmSourceType::Integration:
            return "Integration";
        case AlarmSourceType::Manual:
            return "Manual";
        }

        return "Unknown";
    }

    std::string_view to_string(
        AlarmSeverity severity
    ) noexcept
    {
        switch (severity)
        {
        case AlarmSeverity::Information:
            return "Information";
        case AlarmSeverity::Warning:
            return "Warning";
        case AlarmSeverity::Minor:
            return "Minor";
        case AlarmSeverity::Major:
            return "Major";
        case AlarmSeverity::Critical:
            return "Critical";
        }

        return "Unknown";
    }

    std::string_view to_string(
        AlarmPriority priority
    ) noexcept
    {
        switch (priority)
        {
        case AlarmPriority::Low:
            return "Low";
        case AlarmPriority::Medium:
            return "Medium";
        case AlarmPriority::High:
            return "High";
        case AlarmPriority::Critical:
            return "Critical";
        }

        return "Unknown";
    }

    std::string_view to_string(
        AlarmState state
    ) noexcept
    {
        switch (state)
        {
        case AlarmState::New:
            return "New";
        case AlarmState::Active:
            return "Active";
        case AlarmState::Acknowledged:
            return "Acknowledged";
        case AlarmState::Cleared:
            return "Cleared";
        case AlarmState::Closed:
            return "Closed";
        case AlarmState::Shelved:
            return "Shelved";
        case AlarmState::Suppressed:
            return "Suppressed";
        }

        return "Unknown";
    }
}
