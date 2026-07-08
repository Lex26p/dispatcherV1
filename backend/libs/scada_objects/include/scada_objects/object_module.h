#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::objects
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_object_module_info();
}