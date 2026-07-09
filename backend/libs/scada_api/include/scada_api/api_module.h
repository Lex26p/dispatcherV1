#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::api
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_api_module_info();
}