#include "scada_app/app_module.h"

namespace dispatcher::app
{
    dispatcher::core::ModuleInfo get_app_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_app",
            .name = "Application",
            .description = "Application composition and gateway startup foundation.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}
