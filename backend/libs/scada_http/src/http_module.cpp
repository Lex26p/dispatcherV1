#include "scada_http/http_module.h"

namespace dispatcher::http {

dispatcher::core::ModuleInfo get_http_module_info()
{
    return dispatcher::core::ModuleInfo{
        .code = "scada_http",
        .name = "HTTP",
        .description = "HTTP transport foundation for Dispatcher API gateway.",
        .status = dispatcher::core::ModuleStatus::Running
    };
}

} // namespace dispatcher::http
