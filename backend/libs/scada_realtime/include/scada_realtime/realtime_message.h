#pragma once

#include "scada_realtime/realtime_ids.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>

namespace dispatcher::realtime
{
    using RealtimeTimestamp = std::chrono::system_clock::time_point;

    enum class RealtimeChannelType
    {
        System,
        Runtime,
        Historian,
        Events,
        Alarms,
        Diagnostics,
        Unknown
    };

    enum class RealtimeMessageType
    {
        Heartbeat,
        SystemStatus,
        RuntimeValueChanged,
        HistorySampleWritten,
        EventCreated,
        AlarmCreated,
        AlarmUpdated,
        AlarmCleared,
        AlarmAcknowledged,
        AlarmClosed,
        DiagnosticsUpdated,
        Error,
        Unknown
    };

    enum class RealtimePayloadFormat
    {
        Json,
        Text,
        Binary,
        Empty
    };

    enum class RealtimeDeliveryMode
    {
        Broadcast,
        Channel,
        Client,
        Subscription
    };

    struct RealtimeMessage
    {
        RealtimeMessageId id;

        RealtimeChannelType channel = RealtimeChannelType::Unknown;
        RealtimeMessageType type = RealtimeMessageType::Unknown;
        RealtimePayloadFormat payload_format = RealtimePayloadFormat::Json;
        RealtimeDeliveryMode delivery_mode = RealtimeDeliveryMode::Channel;

        RealtimeClientId client_id;
        RealtimeSubscriptionId subscription_id;

        std::string topic;
        std::string correlation_id;
        std::string payload;

        RealtimeTimestamp timestamp{};

        std::uint64_t sequence_number = 0;

        bool retained = false;

        [[nodiscard]] bool has_message_id() const noexcept;
        [[nodiscard]] bool has_client_id() const noexcept;
        [[nodiscard]] bool has_subscription_id() const noexcept;
        [[nodiscard]] bool has_topic() const noexcept;
        [[nodiscard]] bool has_correlation_id() const noexcept;
        [[nodiscard]] bool has_payload() const noexcept;
        [[nodiscard]] bool has_timestamp() const noexcept;
        [[nodiscard]] bool is_broadcast() const noexcept;
        [[nodiscard]] bool is_valid() const noexcept;
    };

    [[nodiscard]] RealtimeMessage make_realtime_message(
        RealtimeChannelType channel,
        RealtimeMessageType type,
        std::string topic,
        std::string payload
    );

    [[nodiscard]] bool is_empty_timestamp(
        RealtimeTimestamp timestamp
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        RealtimeChannelType channel
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        RealtimeMessageType type
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        RealtimePayloadFormat format
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        RealtimeDeliveryMode mode
    ) noexcept;
}