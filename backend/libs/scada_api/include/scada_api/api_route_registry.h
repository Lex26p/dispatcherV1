#pragma once

#include "scada_api/api_endpoint.h"

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::api
{
    class ApiRouteRegistry
    {
    public:
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;

        [[nodiscard]] const std::vector<ApiEndpoint>& endpoints() const noexcept;

        bool register_endpoint(
            const ApiEndpoint& endpoint
        );

        bool register_endpoint(
            ApiEndpoint&& endpoint
        );

        [[nodiscard]] std::optional<ApiEndpoint> find_by_path(
            ApiHttpMethod method,
            const std::string& path
        ) const;

        [[nodiscard]] std::vector<ApiEndpoint> find_by_area(
            ApiEndpointArea area
        ) const;

        void clear() noexcept;

    private:
        std::vector<ApiEndpoint> endpoints_;

        [[nodiscard]] bool contains(
            ApiHttpMethod method,
            const std::string& path
        ) const;
    };

    [[nodiscard]] ApiRouteRegistry create_default_api_route_registry();
}
