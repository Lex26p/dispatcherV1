#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::devices
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_device_module_info();
}