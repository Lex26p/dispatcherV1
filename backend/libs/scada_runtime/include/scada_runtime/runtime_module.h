#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::runtime
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_runtime_module_info();
}