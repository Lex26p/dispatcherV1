#pragma once

#include "scada_events/event_ids.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>

namespace dispatcher::events
{
    using EventTimestamp = std::chrono::system_clock::time_point;

    enum class EventCategory
    {
        System,
        Runtime,
        Device,
        Tag,
        Communication,
        Historian,
        Security,
        UserAction,
        Integration,
        Maintenance,
        Alarm
    };

    enum class EventSeverity
    {
        Trace,
        Information,
        Warning,
        Error,
        Critical
    };

    enum class EventSourceType
    {
        Unknown,
        System,
        Object,
        Device,
        Tag,
        Runtime,
        Historian,
        Polling,
        Protocol,
        Alarm,
        User,
        Integration,
        Script
    };

    struct EventRecord
    {
        EventId id;

        EventCategory category = EventCategory::System;
        EventSeverity severity = EventSeverity::Information;
        EventSourceType source_type = EventSourceType::Unknown;

        std::string code;
        std::string title;
        std::string message;

        std::string source_id;
        std::string object_id;
        std::string device_id;
        std::string tag_id;

        std::string correlation_id;

        EventTimestamp timestamp{};

        std::uint64_t sequence_number = 0;

        [[nodiscard]] bool has_event_id() const noexcept;
        [[nodiscard]] bool has_code() const noexcept;
        [[nodiscard]] bool has_title() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
        [[nodiscard]] bool has_source_id() const noexcept;
        [[nodiscard]] bool has_object_id() const noexcept;
        [[nodiscard]] bool has_device_id() const noexcept;
        [[nodiscard]] bool has_tag_id() const noexcept;
        [[nodiscard]] bool has_correlation_id() const noexcept;
        [[nodiscard]] bool has_valid_identity() const noexcept;
    };

    [[nodiscard]] EventRecord make_event_record(
        EventCategory category,
        EventSeverity severity,
        EventSourceType source_type,
        std::string code,
        std::string title,
        std::string message
    );

    [[nodiscard]] std::string_view to_string(
        EventCategory category
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        EventSeverity severity
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        EventSourceType source_type
    ) noexcept;
}