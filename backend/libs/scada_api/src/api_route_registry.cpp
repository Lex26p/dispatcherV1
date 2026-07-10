#include "scada_api/api_route_registry.h"

#include <utility>

namespace dispatcher::api
{
    bool ApiRouteRegistry::empty() const noexcept
    {
        return endpoints_.empty();
    }

    std::size_t ApiRouteRegistry::size() const noexcept
    {
        return endpoints_.size();
    }

    const std::vector<ApiEndpoint>& ApiRouteRegistry::endpoints() const noexcept
    {
        return endpoints_;
    }

    bool ApiRouteRegistry::register_endpoint(
        const ApiEndpoint& endpoint
    )
    {
        if (!endpoint.is_valid())
        {
            return false;
        }

        if (contains(endpoint.method, endpoint.path))
        {
            return false;
        }

        endpoints_.push_back(endpoint);

        return true;
    }

    bool ApiRouteRegistry::register_endpoint(
        ApiEndpoint&& endpoint
    )
    {
        if (!endpoint.is_valid())
        {
            return false;
        }

        if (contains(endpoint.method, endpoint.path))
        {
            return false;
        }

        endpoints_.push_back(std::move(endpoint));

        return true;
    }

    std::optional<ApiEndpoint> ApiRouteRegistry::find_by_path(
        ApiHttpMethod method,
        const std::string& path
    ) const
    {
        for (const auto& endpoint : endpoints_)
        {
            if (endpoint.method == method && endpoint.path == path)
            {
                return endpoint;
            }
        }

        return std::nullopt;
    }

    std::vector<ApiEndpoint> ApiRouteRegistry::find_by_area(
        ApiEndpointArea area
    ) const
    {
        std::vector<ApiEndpoint> result;

        for (const auto& endpoint : endpoints_)
        {
            if (endpoint.area == area)
            {
                result.push_back(endpoint);
            }
        }

        return result;
    }

    void ApiRouteRegistry::clear() noexcept
    {
        endpoints_.clear();
    }

    bool ApiRouteRegistry::contains(
        ApiHttpMethod method,
        const std::string& path
    ) const
    {
        return find_by_path(method, path).has_value();
    }

    ApiRouteRegistry create_default_api_route_registry()
    {
        ApiRouteRegistry registry;

        registry.register_endpoint(
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::System,
                "/api/system/health",
                "System health",
                "Returns basic system health information."
            )
        );

        registry.register_endpoint(
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::System,
                "/api/system/modules",
                "System modules",
                "Returns registered backend modules."
            )
        );

        registry.register_endpoint(
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Runtime,
                "/api/runtime/values",
                "Runtime values",
                "Returns current runtime tag values."
            )
        );

        registry.register_endpoint(
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Historian,
                "/api/history/query",
                "History query",
                "Returns historical tag values."
            )
        );

        registry.register_endpoint(
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Events,
                "/api/events",
                "Events",
                "Returns event records."
            )
        );

        registry.register_endpoint(
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Alarms,
                "/api/alarms/active",
                "Active alarms",
                "Returns active alarms."
            )
        );

        registry.register_endpoint(
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Realtime,
                "/api/realtime",
                "Realtime gateway",
                "Future realtime gateway endpoint."
            )
        );

        return registry;
    }
}
