#pragma once

#include "scada_common/id_types.h"
#include "scada_common/result.h"
#include "scada_core/module_info.h"

#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::core
{
    class ApplicationContext
    {
    public:
        ApplicationContext(std::string application_name, std::string executable_name);

        [[nodiscard]] const std::string& application_name() const noexcept;
        [[nodiscard]] const std::string& executable_name() const noexcept;
        [[nodiscard]] const dispatcher::common::CorrelationId& startup_correlation_id() const noexcept;

        dispatcher::common::Result<void> register_module(ModuleInfo module);
        dispatcher::common::Result<void> start();

        [[nodiscard]] bool is_started() const noexcept;
        [[nodiscard]] const std::vector<ModuleInfo>& modules() const noexcept;

    private:
        std::string application_name_;
        std::string executable_name_;
        dispatcher::common::CorrelationId startup_correlation_id_;
        std::vector<ModuleInfo> modules_;
        bool started_ = false;
    };
}
