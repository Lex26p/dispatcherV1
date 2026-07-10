#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::core
{
    enum class ModuleStatus
    {
        NotInitialized,
        Starting,
        Running,
        Degraded,
        Stopped,
        Failed
    };

    struct ModuleInfo
    {
        std::string code;
        std::string name;
        std::string description;
        ModuleStatus status = ModuleStatus::NotInitialized;
    };

    std::string_view to_string(ModuleStatus status);

    std::vector<ModuleInfo> get_initial_module_list();
}
