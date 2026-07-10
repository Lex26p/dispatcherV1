#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::tags
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_tag_module_info();
}
