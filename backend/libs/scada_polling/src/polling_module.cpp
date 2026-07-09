#include "scada_polling/polling_module.h"

namespace dispatcher::polling
{
    dispatcher::core::ModuleInfo get_polling_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_polling",
            .name = "Polling",
            .description = "Polling task, group and scheduler foundation.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}