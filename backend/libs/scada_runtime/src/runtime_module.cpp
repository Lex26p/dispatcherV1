#include "scada_runtime/runtime_module.h"

namespace dispatcher::runtime
{
    dispatcher::core::ModuleInfo get_runtime_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_runtime",
            .name = "Runtime",
            .description = "Runtime tag value store and data engine foundation.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}
