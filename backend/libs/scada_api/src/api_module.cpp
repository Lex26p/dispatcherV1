#include "scada_api/api_module.h"

namespace dispatcher::api
{
    dispatcher::core::ModuleInfo get_api_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_api",
            .name = "API",
            .description = "API route and response foundation.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}