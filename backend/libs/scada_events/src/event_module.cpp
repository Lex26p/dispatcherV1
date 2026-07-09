#include "scada_events/event_module.h"

namespace dispatcher::events
{
    dispatcher::core::ModuleInfo get_event_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_events",
            .name = "Events",
            .description = "Event model and event manager foundation.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}