#include "scada_devices/device_module.h"

namespace dispatcher::devices
{
    dispatcher::core::ModuleInfo get_device_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_devices",
            .name = "Devices",
            .description = "Device model and connection configuration.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}
