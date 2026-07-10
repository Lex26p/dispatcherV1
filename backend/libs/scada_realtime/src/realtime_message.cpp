#include "scada_realtime/realtime_message.h"

#include <chrono>
#include <utility>

namespace dispatcher::realtime
{
    namespace
    {
        [[nodiscard]] RealtimeTimestamp now()
        {
            return std::chrono::system_clock::now();
        }
    }

    bool RealtimeMessage::has_message_id() const noexcept
    {
        return !id.empty();
    }

    bool RealtimeMessage::has_client_id() const noexcept
    {
        return !client_id.empty();
    }

    bool RealtimeMessage::has_subscription_id() const noexcept
    {
        return !subscription_id.empty();
    }

    bool RealtimeMessage::has_topic() const noexcept
    {
        return !topic.empty();
    }

    bool RealtimeMessage::has_correlation_id() const noexcept
    {
        return !correlation_id.empty();
    }

    bool RealtimeMessage::has_payload() const noexcept
    {
        return !payload.empty();
    }

    bool RealtimeMessage::has_timestamp() const noexcept
    {
        return !is_empty_timestamp(timestamp);
    }

    bool RealtimeMessage::is_broadcast() const noexcept
    {
        return delivery_mode == RealtimeDeliveryMode::Broadcast;
    }

    bool RealtimeMessage::is_valid() const noexcept
    {
        return channel != RealtimeChannelType::Unknown &&
            type != RealtimeMessageType::Unknown &&
            has_topic();
    }

    RealtimeMessage make_realtime_message(
        RealtimeChannelType channel,
        RealtimeMessageType type,
        std::string topic,
        std::string payload
    )
    {
        RealtimeMessage message;

        message.channel = channel;
        message.type = type;
        message.topic = std::move(topic);
        message.payload = std::move(payload);
        message.timestamp = now();

        return message;
    }

    bool is_empty_timestamp(
        RealtimeTimestamp timestamp
    ) noexcept
    {
        return timestamp == RealtimeTimestamp{};
    }

    std::string_view to_string(
        RealtimeChannelType channel
    ) noexcept
    {
        switch (channel)
        {
        case RealtimeChannelType::System:
            return "System";
        case RealtimeChannelType::Runtime:
            return "Runtime";
        case RealtimeChannelType::Historian:
            return "Historian";
        case RealtimeChannelType::Events:
            return "Events";
        case RealtimeChannelType::Alarms:
            return "Alarms";
        case RealtimeChannelType::Diagnostics:
            return "Diagnostics";
        case RealtimeChannelType::Unknown:
            return "Unknown";
        }

        return "Unknown";
    }

    std::string_view to_string(
        RealtimeMessageType type
    ) noexcept
    {
        switch (type)
        {
        case RealtimeMessageType::Heartbeat:
            return "Heartbeat";
        case RealtimeMessageType::SystemStatus:
            return "SystemStatus";
        case RealtimeMessageType::RuntimeValueChanged:
            return "RuntimeValueChanged";
        case RealtimeMessageType::HistorySampleWritten:
            return "HistorySampleWritten";
        case RealtimeMessageType::EventCreated:
            return "EventCreated";
        case RealtimeMessageType::AlarmCreated:
            return "AlarmCreated";
        case RealtimeMessageType::AlarmUpdated:
            return "AlarmUpdated";
        case RealtimeMessageType::AlarmCleared:
            return "AlarmCleared";
        case RealtimeMessageType::AlarmAcknowledged:
            return "AlarmAcknowledged";
        case RealtimeMessageType::AlarmClosed:
            return "AlarmClosed";
        case RealtimeMessageType::DiagnosticsUpdated:
            return "DiagnosticsUpdated";
        case RealtimeMessageType::Error:
            return "Error";
        case RealtimeMessageType::Unknown:
            return "Unknown";
        }

        return "Unknown";
    }

    std::string_view to_string(
        RealtimePayloadFormat format
    ) noexcept
    {
        switch (format)
        {
        case RealtimePayloadFormat::Json:
            return "Json";
        case RealtimePayloadFormat::Text:
            return "Text";
        case RealtimePayloadFormat::Binary:
            return "Binary";
        case RealtimePayloadFormat::Empty:
            return "Empty";
        }

        return "Unknown";
    }

    std::string_view to_string(
        RealtimeDeliveryMode mode
    ) noexcept
    {
        switch (mode)
        {
        case RealtimeDeliveryMode::Broadcast:
            return "Broadcast";
        case RealtimeDeliveryMode::Channel:
            return "Channel";
        case RealtimeDeliveryMode::Client:
            return "Client";
        case RealtimeDeliveryMode::Subscription:
            return "Subscription";
        }

        return "Unknown";
    }
}
