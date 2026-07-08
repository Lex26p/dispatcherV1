#include "scada_core/module_info.h"

namespace dispatcher::core
{
    std::string_view to_string(ModuleStatus status)
    {
        switch (status)
        {
        case ModuleStatus::NotInitialized:
            return "NotInitialized";
        case ModuleStatus::Starting:
            return "Starting";
        case ModuleStatus::Running:
            return "Running";
        case ModuleStatus::Degraded:
            return "Degraded";
        case ModuleStatus::Stopped:
            return "Stopped";
        case ModuleStatus::Failed:
            return "Failed";
        }

        return "Unknown";
    }

    std::vector<ModuleInfo> get_initial_module_list()
    {
        return {
            ModuleInfo{
                .code = "scada_common",
                .name = "Common",
                .description = "Common types and utilities.",
                .status = ModuleStatus::Running
            },
            ModuleInfo{
                .code = "scada_contracts",
                .name = "Contracts",
                .description = "API and exchange contracts.",
                .status = ModuleStatus::Running
            },
            ModuleInfo{
                .code = "scada_core",
                .name = "Core",
                .description = "Core application abstractions.",
                .status = ModuleStatus::Running
            }
        };
    }
}