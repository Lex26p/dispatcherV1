#include "scada_api/api_endpoint.h"

#include <utility>

namespace dispatcher::api
{
    bool ApiEndpoint::has_path() const noexcept
    {
        return !path.empty();
    }

    bool ApiEndpoint::has_name() const noexcept
    {
        return !name.empty();
    }

    bool ApiEndpoint::has_description() const noexcept
    {
        return !description.empty();
    }

    bool ApiEndpoint::is_public() const noexcept
    {
        return !requires_authentication;
    }

    bool ApiEndpoint::is_valid() const noexcept
    {
        return has_path() && has_name();
    }

    ApiEndpoint make_api_endpoint(
        ApiHttpMethod method,
        ApiEndpointArea area,
        std::string path,
        std::string name,
        std::string description
    )
    {
        ApiEndpoint endpoint;

        endpoint.method = method;
        endpoint.area = area;
        endpoint.path = std::move(path);
        endpoint.name = std::move(name);
        endpoint.description = std::move(description);

        return endpoint;
    }

    std::string_view to_string(
        ApiHttpMethod method
    ) noexcept
    {
        switch (method)
        {
        case ApiHttpMethod::Get:
            return "GET";
        case ApiHttpMethod::Post:
            return "POST";
        case ApiHttpMethod::Put:
            return "PUT";
        case ApiHttpMethod::Patch:
            return "PATCH";
        case ApiHttpMethod::Delete:
            return "DELETE";
        }

        return "UNKNOWN";
    }

    std::string_view to_string(
        ApiEndpointArea area
    ) noexcept
    {
        switch (area)
        {
        case ApiEndpointArea::System:
            return "System";
        case ApiEndpointArea::Objects:
            return "Objects";
        case ApiEndpointArea::Devices:
            return "Devices";
        case ApiEndpointArea::Tags:
            return "Tags";
        case ApiEndpointArea::Runtime:
            return "Runtime";
        case ApiEndpointArea::Historian:
            return "Historian";
        case ApiEndpointArea::Events:
            return "Events";
        case ApiEndpointArea::Alarms:
            return "Alarms";
        case ApiEndpointArea::Realtime:
            return "Realtime";
        case ApiEndpointArea::Unknown:
            return "Unknown";
        }

        return "Unknown";
    }
}