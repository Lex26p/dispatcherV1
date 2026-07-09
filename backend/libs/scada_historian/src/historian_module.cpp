#include "scada_historian/historian_module.h"

namespace dispatcher::historian
{
    dispatcher::core::ModuleInfo get_historian_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_historian",
            .name = "Historian",
            .description = "Historian sample model and archive foundation.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}