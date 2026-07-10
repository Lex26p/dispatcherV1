#include "scada_events/event_record.h"

#include <chrono>
#include <utility>

namespace dispatcher::events
{
    namespace
    {
        [[nodiscard]] EventTimestamp now()
        {
            return std::chrono::system_clock::now();
        }
    }

    bool EventRecord::has_event_id() const noexcept
    {
        return !id.empty();
    }

    bool EventRecord::has_code() const noexcept
    {
        return !code.empty();
    }

    bool EventRecord::has_title() const noexcept
    {
        return !title.empty();
    }

    bool EventRecord::has_message() const noexcept
    {
        return !message.empty();
    }

    bool EventRecord::has_source_id() const noexcept
    {
        return !source_id.empty();
    }

    bool EventRecord::has_object_id() const noexcept
    {
        return !object_id.empty();
    }

    bool EventRecord::has_device_id() const noexcept
    {
        return !device_id.empty();
    }

    bool EventRecord::has_tag_id() const noexcept
    {
        return !tag_id.empty();
    }

    bool EventRecord::has_correlation_id() const noexcept
    {
        return !correlation_id.empty();
    }

    bool EventRecord::has_valid_identity() const noexcept
    {
        return has_code() && has_title();
    }

    EventRecord make_event_record(
        EventCategory category,
        EventSeverity severity,
        EventSourceType source_type,
        std::string code,
        std::string title,
        std::string message
    )
    {
        EventRecord event;

        event.category = category;
        event.severity = severity;
        event.source_type = source_type;
        event.code = std::move(code);
        event.title = std::move(title);
        event.message = std::move(message);
        event.timestamp = now();

        return event;
    }

    std::string_view to_string(
        EventCategory category
    ) noexcept
    {
        switch (category)
        {
        case EventCategory::System:
            return "System";
        case EventCategory::Runtime:
            return "Runtime";
        case EventCategory::Device:
            return "Device";
        case EventCategory::Tag:
            return "Tag";
        case EventCategory::Communication:
            return "Communication";
        case EventCategory::Historian:
            return "Historian";
        case EventCategory::Security:
            return "Security";
        case EventCategory::UserAction:
            return "UserAction";
        case EventCategory::Integration:
            return "Integration";
        case EventCategory::Maintenance:
            return "Maintenance";
        case EventCategory::Alarm:
            return "Alarm";
        }

        return "Unknown";
    }

    std::string_view to_string(
        EventSeverity severity
    ) noexcept
    {
        switch (severity)
        {
        case EventSeverity::Trace:
            return "Trace";
        case EventSeverity::Information:
            return "Information";
        case EventSeverity::Warning:
            return "Warning";
        case EventSeverity::Error:
            return "Error";
        case EventSeverity::Critical:
            return "Critical";
        }

        return "Unknown";
    }

    std::string_view to_string(
        EventSourceType source_type
    ) noexcept
    {
        switch (source_type)
        {
        case EventSourceType::Unknown:
            return "Unknown";
        case EventSourceType::System:
            return "System";
        case EventSourceType::Object:
            return "Object";
        case EventSourceType::Device:
            return "Device";
        case EventSourceType::Tag:
            return "Tag";
        case EventSourceType::Runtime:
            return "Runtime";
        case EventSourceType::Historian:
            return "Historian";
        case EventSourceType::Polling:
            return "Polling";
        case EventSourceType::Protocol:
            return "Protocol";
        case EventSourceType::Alarm:
            return "Alarm";
        case EventSourceType::User:
            return "User";
        case EventSourceType::Integration:
            return "Integration";
        case EventSourceType::Script:
            return "Script";
        }

        return "Unknown";
    }
}
