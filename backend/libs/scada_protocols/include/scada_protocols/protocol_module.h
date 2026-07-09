#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::protocols
{
    [[nodiscard]] dispatcher::core::ModuleInfo get_protocol_module_info();
}