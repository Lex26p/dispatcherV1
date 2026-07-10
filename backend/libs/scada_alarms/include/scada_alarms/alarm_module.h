#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::alarms
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_alarm_module_info();
}
