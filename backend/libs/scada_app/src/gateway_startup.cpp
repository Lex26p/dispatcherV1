#include "scada_app/gateway_startup.h"

namespace dispatcher::app
{
    bool GatewayStartupOptions::has_bind_address() const noexcept
    {
        return !bind_address.empty();
    }

    bool GatewayStartupOptions::has_api_base_path() const noexcept
    {
        return !api_base_path.empty();
    }

    bool GatewayStartupOptions::has_realtime_path() const noexcept
    {
        return !realtime_path.empty();
    }

    bool GatewayStartupOptions::api_enabled() const noexcept
    {
        return enable_api &&
            (mode == GatewayMode::ApiOnly ||
                mode == GatewayMode::ApiAndRealtime);
    }

    bool GatewayStartupOptions::realtime_enabled() const noexcept
    {
        return enable_realtime &&
            (mode == GatewayMode::RealtimeOnly ||
                mode == GatewayMode::ApiAndRealtime);
    }

    bool GatewayStartupOptions::is_valid() const noexcept
    {
        if (mode == GatewayMode::Disabled)
        {
            return true;
        }

        if (!has_bind_address())
        {
            return false;
        }

        if (api_enabled() && !has_api_base_path())
        {
            return false;
        }

        if (realtime_enabled() && !has_realtime_path())
        {
            return false;
        }

        if (http_port == 0)
        {
            return false;
        }

        if (realtime_enabled() && realtime_port == 0)
        {
            return false;
        }

        return true;
    }

    bool GatewayStartupPlan::is_prepared() const noexcept
    {
        return status == GatewayStartupStatus::Prepared;
    }

    bool GatewayStartupPlan::is_running() const noexcept
    {
        return status == GatewayStartupStatus::Running;
    }

    bool GatewayStartupPlan::has_message() const noexcept
    {
        return !message.empty();
    }

    bool GatewayStartupPlan::api_enabled() const noexcept
    {
        return options.api_enabled();
    }

    bool GatewayStartupPlan::realtime_enabled() const noexcept
    {
        return options.realtime_enabled();
    }

    GatewayStartupPlan create_gateway_startup_plan(
        const GatewayStartupOptions& options,
        std::uint64_t api_route_count,
        std::uint64_t api_read_endpoint_count,
        std::uint64_t realtime_channel_count
    )
    {
        GatewayStartupPlan plan;

        plan.options = options;
        plan.api_route_count = api_route_count;
        plan.api_read_endpoint_count = api_read_endpoint_count;
        plan.realtime_channel_count = realtime_channel_count;

        if (!options.is_valid())
        {
            plan.status = GatewayStartupStatus::Failed;
            plan.message = "Gateway startup options are invalid.";

            return plan;
        }

        if (options.mode == GatewayMode::Disabled)
        {
            plan.status = GatewayStartupStatus::Stopped;
            plan.message = "Gateway startup is disabled.";

            return plan;
        }

        plan.status = GatewayStartupStatus::Prepared;
        plan.message = "Gateway startup plan prepared.";

        return plan;
    }

    std::string_view to_string(
        GatewayMode mode
    ) noexcept
    {
        switch (mode)
        {
        case GatewayMode::Disabled:
            return "Disabled";
        case GatewayMode::ApiOnly:
            return "ApiOnly";
        case GatewayMode::RealtimeOnly:
            return "RealtimeOnly";
        case GatewayMode::ApiAndRealtime:
            return "ApiAndRealtime";
        }

        return "Unknown";
    }

    std::string_view to_string(
        GatewayStartupStatus status
    ) noexcept
    {
        switch (status)
        {
        case GatewayStartupStatus::NotStarted:
            return "NotStarted";
        case GatewayStartupStatus::Prepared:
            return "Prepared";
        case GatewayStartupStatus::Running:
            return "Running";
        case GatewayStartupStatus::Stopped:
            return "Stopped";
        case GatewayStartupStatus::Failed:
            return "Failed";
        }

        return "Unknown";
    }
}
