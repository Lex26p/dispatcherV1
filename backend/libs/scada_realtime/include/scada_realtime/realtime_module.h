#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::realtime
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_realtime_module_info();
}