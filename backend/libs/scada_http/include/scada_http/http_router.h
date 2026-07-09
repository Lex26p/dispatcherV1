#pragma once

#include "scada_http/http_types.h"

#include <functional>
#include <vector>

namespace dispatcher::http {

using HttpRouteHandler = std::function<HttpResponse(const HttpRequest& request)>;

struct HttpRoute {
    HttpEndpoint endpoint;
    HttpRouteHandler handler;

    [[nodiscard]] bool has_handler() const noexcept;
    [[nodiscard]] bool is_valid() const noexcept;
};

class HttpRouteDispatcher {
public:
    [[nodiscard]] bool add_route(
        HttpEndpoint endpoint,
        HttpRouteHandler handler
    );

    [[nodiscard]] HttpResponse dispatch(
        const HttpRequest& request
    ) const;

    [[nodiscard]] bool has_route(
        HttpMethod method,
        const std::string& path
    ) const;

    [[nodiscard]] bool has_path(
        const std::string& path
    ) const;

    [[nodiscard]] std::size_t route_count() const noexcept;

    [[nodiscard]] const std::vector<HttpRoute>& routes() const noexcept;

private:
    std::vector<HttpRoute> routes_;
};

[[nodiscard]] HttpResponse make_bad_request_response();

[[nodiscard]] HttpResponse make_not_found_response(
    const std::string& path
);

[[nodiscard]] HttpResponse make_method_not_allowed_response(
    const std::string& path,
    HttpMethod method
);

} // namespace dispatcher::http
