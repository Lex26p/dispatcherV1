#pragma once

#include "scada_api/configuration_read_service.h"

#include "scada_http/http_router.h"

#include <string_view>

namespace dispatcher::http {

[[nodiscard]] std::string_view
tags_endpoint_path() noexcept;

[[nodiscard]] HttpEndpoint
make_tags_endpoint();

[[nodiscard]] bool register_tag_read_endpoint(
    HttpRouteDispatcher& route_dispatcher,
    dispatcher::api::ConfigurationReadServicePtr read_service
);

} // namespace dispatcher::http
