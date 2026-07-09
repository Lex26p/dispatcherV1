#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace dispatcher::app
{
    enum class GatewayMode
    {
        Disabled,
        ApiOnly,
        RealtimeOnly,
        ApiAndRealtime
    };

    enum class GatewayStartupStatus
    {
        NotStarted,
        Prepared,
        Running,
        Stopped,
        Failed
    };

    struct GatewayStartupOptions
    {
        GatewayMode mode = GatewayMode::ApiAndRealtime;

        std::string bind_address = "127.0.0.1";

        std::uint16_t http_port = 8080;
        std::uint16_t realtime_port = 8080;

        std::string api_base_path = "/api";
        std::string realtime_path = "/api/realtime";

        bool enable_api = true;
        bool enable_realtime = true;
        bool require_authentication = false;
        bool allow_loopback_only = true;

        [[nodiscard]] bool has_bind_address() const noexcept;
        [[nodiscard]] bool has_api_base_path() const noexcept;
        [[nodiscard]] bool has_realtime_path() const noexcept;
        [[nodiscard]] bool api_enabled() const noexcept;
        [[nodiscard]] bool realtime_enabled() const noexcept;
        [[nodiscard]] bool is_valid() const noexcept;
    };

    struct GatewayStartupPlan
    {
        GatewayStartupOptions options;

        GatewayStartupStatus status = GatewayStartupStatus::NotStarted;

        std::uint64_t api_route_count = 0;
        std::uint64_t api_read_endpoint_count = 0;
        std::uint64_t realtime_channel_count = 0;

        std::string message;

        [[nodiscard]] bool is_prepared() const noexcept;
        [[nodiscard]] bool is_running() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
        [[nodiscard]] bool api_enabled() const noexcept;
        [[nodiscard]] bool realtime_enabled() const noexcept;
    };

    [[nodiscard]] GatewayStartupPlan create_gateway_startup_plan(
        const GatewayStartupOptions& options,
        std::uint64_t api_route_count,
        std::uint64_t api_read_endpoint_count,
        std::uint64_t realtime_channel_count
    );

    [[nodiscard]] std::string_view to_string(
        GatewayMode mode
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        GatewayStartupStatus status
    ) noexcept;
}