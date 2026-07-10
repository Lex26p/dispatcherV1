#pragma once

#include "scada_api/configuration_read_service.h"

#include "scada_http/http_router.h"

#include <string_view>

namespace dispatcher::http {

[[nodiscard]] std::string_view
objects_endpoint_path() noexcept;

[[nodiscard]] std::string_view
object_tree_endpoint_path() noexcept;

[[nodiscard]] HttpEndpoint
make_objects_endpoint();

[[nodiscard]] HttpEndpoint
make_object_tree_endpoint();

[[nodiscard]] bool register_object_read_endpoints(
    HttpRouteDispatcher& route_dispatcher,
    dispatcher::api::ConfigurationReadServicePtr
        read_service
);

} // namespace dispatcher::http
