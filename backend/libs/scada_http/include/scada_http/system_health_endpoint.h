#pragma once

#include "scada_http/http_router.h"

#include <string_view>

namespace dispatcher::http {

[[nodiscard]] std::string_view system_health_endpoint_path() noexcept;

[[nodiscard]] HttpEndpoint make_system_health_endpoint();

[[nodiscard]] HttpResponse make_system_health_response();

[[nodiscard]] bool register_system_health_endpoint(
    HttpRouteDispatcher& route_dispatcher
);

} // namespace dispatcher::http
