#pragma once

#include "scada_core/module_info.h"

namespace dispatcher::http {

[[nodiscard]] dispatcher::core::ModuleInfo get_http_module_info();

} // namespace dispatcher::http
