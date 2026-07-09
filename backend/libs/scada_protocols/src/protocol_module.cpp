#include "scada_protocols/protocol_module.h"

namespace dispatcher::protocols
{
    dispatcher::core::ModuleInfo get_protocol_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_protocols",
            .name = "Protocols",
            .description = "Protocol driver contracts and communication primitives.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}