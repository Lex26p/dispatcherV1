#pragma once

#include "scada_core/module_info.h"
#include "scada_http/http_router.h"

#include <vector>

namespace dispatcher::http {

[[nodiscard]] HttpRouteDispatcher make_system_route_dispatcher(
    const std::vector<dispatcher::core::ModuleInfo>& modules
);

} // namespace dispatcher::http
