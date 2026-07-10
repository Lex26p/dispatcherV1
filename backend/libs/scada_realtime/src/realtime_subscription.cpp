#include "scada_realtime/realtime_subscription.h"

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

    bool RealtimeSubscription::has_subscription_id() const noexcept
    {
        return !id.empty();
    }

    bool RealtimeSubscription::has_client_id() const noexcept
    {
        return !client_id.empty();
    }

    bool RealtimeSubscription::has_topic() const noexcept
    {
        return !topic.empty();
    }

    bool RealtimeSubscription::has_filter() const noexcept
    {
        return !filter.empty();
    }

    bool RealtimeSubscription::has_created_at() const noexcept
    {
        return !is_empty_timestamp(created_at);
    }

    bool RealtimeSubscription::has_last_message_at() const noexcept
    {
        return !is_empty_timestamp(last_message_at);
    }

    bool RealtimeSubscription::is_active() const noexcept
    {
        return state == RealtimeSubscriptionState::Active;
    }

    bool RealtimeSubscription::is_closed() const noexcept
    {
        return state == RealtimeSubscriptionState::Closed;
    }

    bool RealtimeSubscription::is_valid() const noexcept
    {
        return has_client_id() &&
            channel != RealtimeChannelType::Unknown &&
            has_topic();
    }

    RealtimeSubscription make_realtime_subscription(
        RealtimeClientId client_id,
        RealtimeChannelType channel,
        std::string topic
    )
    {
        RealtimeSubscription subscription;

        subscription.client_id = client_id;
        subscription.channel = channel;
        subscription.topic = std::move(topic);
        subscription.state = RealtimeSubscriptionState::Active;
        subscription.created_at = now();

        return subscription;
    }

    std::string_view to_string(
        RealtimeSubscriptionState state
    ) noexcept
    {
        switch (state)
        {
        case RealtimeSubscriptionState::Active:
            return "Active";
        case RealtimeSubscriptionState::Paused:
            return "Paused";
        case RealtimeSubscriptionState::Closed:
            return "Closed";
        }

        return "Unknown";
    }
}
