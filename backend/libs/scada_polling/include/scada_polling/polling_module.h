#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::polling
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_polling_module_info();
}