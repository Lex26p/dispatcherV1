#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::historian
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_historian_module_info();
}