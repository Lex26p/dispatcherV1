#pragma once

#include "scada_realtime/realtime_message.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace dispatcher::realtime
{
    enum class RealtimeSubscriptionState
    {
        Active,
        Paused,
        Closed
    };

    struct RealtimeSubscription
    {
        RealtimeSubscriptionId id;
        RealtimeClientId client_id;

        RealtimeChannelType channel = RealtimeChannelType::Unknown;

        std::string topic;
        std::string filter;

        RealtimeSubscriptionState state = RealtimeSubscriptionState::Active;

        RealtimeTimestamp created_at{};
        RealtimeTimestamp last_message_at{};

        std::uint64_t delivered_count = 0;
        std::uint64_t dropped_count = 0;

        bool include_snapshot = true;

        [[nodiscard]] bool has_subscription_id() const noexcept;
        [[nodiscard]] bool has_client_id() const noexcept;
        [[nodiscard]] bool has_topic() const noexcept;
        [[nodiscard]] bool has_filter() const noexcept;
        [[nodiscard]] bool has_created_at() const noexcept;
        [[nodiscard]] bool has_last_message_at() const noexcept;
        [[nodiscard]] bool is_active() const noexcept;
        [[nodiscard]] bool is_closed() const noexcept;
        [[nodiscard]] bool is_valid() const noexcept;
    };

    [[nodiscard]] RealtimeSubscription make_realtime_subscription(
        RealtimeClientId client_id,
        RealtimeChannelType channel,
        std::string topic
    );

    [[nodiscard]] std::string_view to_string(
        RealtimeSubscriptionState state
    ) noexcept;
}
