#include "scada_http/system_endpoint_registry.h"

#include "scada_http/system_health_endpoint.h"
#include "scada_http/system_modules_endpoint.h"

namespace dispatcher::http {

HttpRouteDispatcher make_system_route_dispatcher(
    const std::vector<dispatcher::core::ModuleInfo>& modules
)
{
    HttpRouteDispatcher route_dispatcher;

    const auto health_registered = register_system_health_endpoint(route_dispatcher);
    const auto modules_registered = register_system_modules_endpoint(route_dispatcher, modules);

    (void)health_registered;
    (void)modules_registered;

    return route_dispatcher;
}

} // namespace dispatcher::http
