#include "scada_app/application_composition.h"

namespace dispatcher::app
{
    namespace
    {
        constexpr std::uint64_t default_realtime_channel_count = 6;
    }

    bool ApplicationComposition::has_name() const noexcept
    {
        return !name.empty();
    }

    bool ApplicationComposition::has_environment() const noexcept
    {
        return !environment.empty();
    }

    bool ApplicationComposition::has_api_routes() const noexcept
    {
        return !api_routes.empty();
    }

    bool ApplicationComposition::has_api_read_endpoints() const noexcept
    {
        return !api_read_endpoints.empty();
    }

    bool ApplicationComposition::is_ready() const noexcept
    {
        return ready;
    }

    std::uint64_t ApplicationComposition::api_route_count() const noexcept
    {
        return static_cast<std::uint64_t>(api_routes.size());
    }

    std::uint64_t ApplicationComposition::api_read_endpoint_count() const noexcept
    {
        return static_cast<std::uint64_t>(api_read_endpoints.size());
    }

    ApplicationComposition create_default_application_composition(
        const GatewayStartupOptions& options
    )
    {
        ApplicationComposition composition;

        composition.api_routes = dispatcher::api::create_default_api_route_registry();
        composition.api_read_endpoints = dispatcher::api::create_default_api_read_endpoint_catalog();

        composition.api_configured = composition.has_api_routes() &&
            composition.has_api_read_endpoints();

        composition.realtime_configured = options.realtime_enabled();

        composition.gateway_startup_plan = create_gateway_startup_plan(
            options,
            composition.api_route_count(),
            composition.api_read_endpoint_count(),
            default_realtime_channel_count
        );

        composition.ready = composition.api_configured &&
            composition.gateway_startup_plan.is_prepared();

        return composition;
    }
}
