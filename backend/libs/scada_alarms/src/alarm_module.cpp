#include "scada_alarms/alarm_module.h"

namespace dispatcher::alarms
{
    dispatcher::core::ModuleInfo get_alarm_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_alarms",
            .name = "Alarms",
            .description = "Alarm model and alarm manager foundation.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}