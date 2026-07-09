#pragma once

#include "scada_core/module_info.h"
#include "scada_http/http_router.h"

#include <string_view>
#include <vector>

namespace dispatcher::http {

[[nodiscard]] std::string_view system_modules_endpoint_path() noexcept;

[[nodiscard]] HttpEndpoint make_system_modules_endpoint();

[[nodiscard]] HttpResponse make_system_modules_response(
    const std::vector<dispatcher::core::ModuleInfo>& modules
);

[[nodiscard]] bool register_system_modules_endpoint(
    HttpRouteDispatcher& route_dispatcher,
    std::vector<dispatcher::core::ModuleInfo> modules
);

} // namespace dispatcher::http
