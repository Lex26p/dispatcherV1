#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::events
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_event_module_info();
}