#pragma once

#include "scada_common/result.h"
#include "scada_core/module_info.h"

namespace dispatcher::core
{
    class IModule
    {
    public:
        virtual ~IModule() = default;

        [[nodiscard]] virtual ModuleInfo info() const = 0;

        virtual dispatcher::common::Result<void> start() = 0;
        virtual dispatcher::common::Result<void> stop() = 0;
    };
}
