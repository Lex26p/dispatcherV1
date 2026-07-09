#include "scada_realtime/realtime_module.h"

namespace dispatcher::realtime
{
    dispatcher::core::ModuleInfo get_realtime_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_realtime",
            .name = "Realtime",
            .description = "Realtime message and subscription foundation.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}