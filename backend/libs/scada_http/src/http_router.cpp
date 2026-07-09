#include "scada_http/http_router.h"

#include <sstream>
#include <utility>

namespace dispatcher::http {
namespace {

[[nodiscard]] std::string json_escape(
    const std::string& value
)
{
    std::ostringstream output;

    for (const auto character : value) {
        switch (character) {
        case '"':
            output << "\\\"";
            break;
        case '\\':
            output << "\\\\";
            break;
        case '\b':
            output << "\\b";
            break;
        case '\f':
            output << "\\f";
            break;
        case '\n':
            output << "\\n";
            break;
        case '\r':
            output << "\\r";
            break;
        case '\t':
            output << "\\t";
            break;
        default:
            output << character;
            break;
        }
    }

    return output.str();
}

[[nodiscard]] std::string make_error_json(
    const std::string& code,
    const std::string& message
)
{
    std::ostringstream output;

    output
        << "{"
        << "\"error\":{"
        << "\"code\":\"" << json_escape(code) << "\","
        << "\"message\":\"" << json_escape(message) << "\""
        << "}"
        << "}";

    return output.str();
}

} // namespace

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

    if (has_route(route.endpoint.method, route.endpoint.path)) {
        return false;
    }

    routes_.push_back(std::move(route));

    return true;
}

HttpResponse HttpRouteDispatcher::dispatch(
    const HttpRequest& request
) const
{
    if (!request.is_valid()) {
        return make_bad_request_response();
    }

    bool path_exists = false;

    for (const auto& route : routes_) {
        if (route.endpoint.path != request.path) {
            continue;
        }

        path_exists = true;

        if (route.endpoint.method != request.method) {
            continue;
        }

        return route.handler(request);
    }

    if (path_exists) {
        return make_method_not_allowed_response(
            request.path,
            request.method
        );
    }

    return make_not_found_response(request.path);
}

bool HttpRouteDispatcher::has_route(
    HttpMethod method,
    const std::string& path
) const
{
    for (const auto& route : routes_) {
        if (route.endpoint.method == method && route.endpoint.path == path) {
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

const std::vector<HttpRoute>& HttpRouteDispatcher::routes() const noexcept
{
    return routes_;
}

HttpResponse make_bad_request_response()
{
    return make_json_response(
        HttpStatusCode::BadRequest,
        make_error_json(
            "bad_request",
            "HTTP request is invalid."
        )
    );
}

HttpResponse make_not_found_response(
    const std::string& path
)
{
    return make_json_response(
        HttpStatusCode::NotFound,
        make_error_json(
            "not_found",
            "Route was not found: " + path
        )
    );
}

HttpResponse make_method_not_allowed_response(
    const std::string& path,
    HttpMethod method
)
{
    return make_json_response(
        HttpStatusCode::MethodNotAllowed,
        make_error_json(
            "method_not_allowed",
            "HTTP method " + std::string(to_string(method)) + " is not allowed for route: " + path
        )
    );
}

} // namespace dispatcher::http
