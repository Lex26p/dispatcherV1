#pragma once

#include "scada_api/api_read_endpoint.h"
#include "scada_api/api_route_registry.h"
#include "scada_app/gateway_startup.h"

#include <cstdint>
#include <string>

namespace dispatcher::app
{
    struct ApplicationComposition
    {
        dispatcher::api::ApiRouteRegistry api_routes;
        dispatcher::api::ApiReadEndpointCatalog api_read_endpoints;

        GatewayStartupPlan gateway_startup_plan;

        std::string name = "Dispatcher";
        std::string environment = "Development";

        bool api_configured = false;
        bool realtime_configured = false;
        bool ready = false;

        [[nodiscard]] bool has_name() const noexcept;
        [[nodiscard]] bool has_environment() const noexcept;
        [[nodiscard]] bool has_api_routes() const noexcept;
        [[nodiscard]] bool has_api_read_endpoints() const noexcept;
        [[nodiscard]] bool is_ready() const noexcept;

        [[nodiscard]] std::uint64_t api_route_count() const noexcept;
        [[nodiscard]] std::uint64_t api_read_endpoint_count() const noexcept;
    };

    [[nodiscard]] ApplicationComposition create_default_application_composition(
        const GatewayStartupOptions& options = GatewayStartupOptions{}
    );
}