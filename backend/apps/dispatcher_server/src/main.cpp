#include "scada_common/version.h"
#include "scada_contracts/api_envelope.h"
#include "scada_core/module_info.h"

#include <cstdlib>
#include <iostream>

int main()
{
    const auto version = dispatcher::common::get_version_info();
    const auto startup_envelope = dispatcher::contracts::make_success_envelope(
        "startup",
        "Dispatcher server startup initialized"
    );
    const auto modules = dispatcher::core::get_initial_module_list();

    std::cout << version.product_name << " server" << '\n';
    std::cout << "Executable: " << version.executable_name << '\n';
    std::cout << "Version: " << version.version << '\n';
    std::cout << "Mode: " << version.mode << '\n';
    std::cout << "Status: starting" << '\n';
    std::cout << "Startup message: " << startup_envelope.message << '\n';
    std::cout << "Modules:" << '\n';

    for (const auto& module : modules)
    {
        std::cout << "  - " << module.code
            << " | " << module.name
            << " | " << dispatcher::core::to_string(module.status)
            << '\n';
    }

    return EXIT_SUCCESS;
}