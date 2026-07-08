#include "scada_objects/object_module.h"

namespace dispatcher::objects
{
    dispatcher::core::ModuleInfo get_object_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_objects",
            .name = "Objects",
            .description = "Object model and location hierarchy.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}