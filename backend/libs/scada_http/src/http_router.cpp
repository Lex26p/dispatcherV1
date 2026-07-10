#include "scada_http/http_router.h"

#include "scada_http/api_error.h"
#include "scada_http/correlation_id.h"
#include "scada_http/json_value.h"

#include <exception>
#include <string>
#include <utility>

namespace dispatcher::http {

bool HttpRoute::has_handler() const noexcept
{
    return static_cast<bool>(handler);
}

bool HttpRoute::is_valid() const noexcept
{
    return endpoint.is_valid()
        && has_handler();
}

bool HttpRouteDispatcher::add_route(
    HttpEndpoint endpoint,
    HttpRouteHandler handler
)
{
    HttpRoute route{
        .endpoint = std::move(endpoint),
        .handler = std::move(handler)
    };

    if (!route.is_valid()) {
        return false;
    }

    if (
        has_route(
            route.endpoint.method,
            route.endpoint.path
        )
    ) {
        return false;
    }

    routes_.push_back(
        std::move(route)
    );

    return true;
}

HttpResponse HttpRouteDispatcher::dispatch(
    const HttpRequest& request
) const
{
    auto normalized_request = request;

    normalized_request.correlation_id =
        resolve_correlation_id(request);

    if (!normalized_request.is_valid()) {
        return make_bad_request_response(
            normalized_request.correlation_id
        );
    }

    bool path_exists = false;

    for (const auto& route : routes_) {
        if (
            route.endpoint.path
            != normalized_request.path
        ) {
            continue;
        }

        path_exists = true;

        if (
            route.endpoint.method
            != normalized_request.method
        ) {
            continue;
        }

        try {
            auto response = route.handler(
                normalized_request
            );

            set_correlation_id_header(
                response,
                normalized_request.correlation_id
            );

            return response;
        }
        catch (const std::exception&) {
            return make_internal_server_error_response(
                normalized_request.correlation_id
            );
        }
        catch (...) {
            return make_internal_server_error_response(
                normalized_request.correlation_id
            );
        }
    }

    if (path_exists) {
        return make_method_not_allowed_response(
            normalized_request.path,
            normalized_request.method,
            normalized_request.correlation_id
        );
    }

    return make_not_found_response(
        normalized_request.path,
        normalized_request.correlation_id
    );
}

bool HttpRouteDispatcher::has_route(
    const HttpMethod method,
    const std::string& path
) const
{
    for (const auto& route : routes_) {
        if (
            route.endpoint.method == method
            && route.endpoint.path == path
        ) {
            return true;
        }
    }

    return false;
}

bool HttpRouteDispatcher::has_path(
    const std::string& path
) const
{
    for (const auto& route : routes_) {
        if (route.endpoint.path == path) {
            return true;
        }
    }

    return false;
}

std::size_t HttpRouteDispatcher::route_count() const noexcept
{
    return routes_.size();
}

const std::vector<HttpRoute>&
HttpRouteDispatcher::routes() const noexcept
{
    return routes_;
}

HttpResponse make_bad_request_response(
    const std::string_view correlation_id
)
{
    return make_api_error_response(
        HttpStatusCode::BadRequest,
        "bad_request",
        "HTTP request is invalid.",
        correlation_id
    );
}

HttpResponse make_not_found_response(
    const std::string& path,
    const std::string_view correlation_id
)
{
    auto details = JsonValue::object();

    details.set_string(
        "path",
        path
    );

    return make_api_error_response(
        HttpStatusCode::NotFound,
        "not_found",
        "The requested API route was not found.",
        correlation_id,
        std::move(details)
    );
}

HttpResponse make_method_not_allowed_response(
    const std::string& path,
    const HttpMethod method,
    const std::string_view correlation_id
)
{
    auto details = JsonValue::object();

    details
        .set_string(
            "path",
            path
        )
        .set_string(
            "method",
            to_string(method)
        );

    return make_api_error_response(
        HttpStatusCode::MethodNotAllowed,
        "method_not_allowed",
        "The HTTP method is not allowed for this API route.",
        correlation_id,
        std::move(details)
    );
}

HttpResponse make_internal_server_error_response(
    const std::string_view correlation_id
)
{
    return make_api_error_response(
        HttpStatusCode::InternalServerError,
        "internal_server_error",
        "An internal server error occurred.",
        correlation_id
    );
}

} // namespace dispatcher::http
