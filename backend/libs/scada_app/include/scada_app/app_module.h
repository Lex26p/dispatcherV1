#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::app
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_app_module_info();
}
