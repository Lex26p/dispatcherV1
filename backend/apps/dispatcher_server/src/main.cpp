#include "scada_common/error.h"
#include "scada_common/id_types.h"
#include "scada_common/result.h"
#include "scada_common/timestamp.h"
#include "scada_common/version.h"
#include "scada_contracts/api_envelope.h"
#include "scada_core/module_info.h"

#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
    dispatcher::common::Result<std::string> validate_startup_mode(std::string mode)
    {
        if (mode.empty())
        {
            return dispatcher::common::Result<std::string>::failure(
                dispatcher::common::make_error(
                    dispatcher::common::ErrorCode::ValidationFailed,
                    "Startup mode must not be empty"
                )
            );
        }

        return dispatcher::common::Result<std::string>::success(std::move(mode));
    }
}

int main()
{
    const auto version = dispatcher::common::get_version_info();
    const auto correlation_id = dispatcher::common::CorrelationId::startup();
    const auto startup_time = dispatcher::common::now_utc();

    const auto startup_mode = validate_startup_mode(std::string{ version.mode });
    if (startup_mode.is_failure())
    {
        std::cerr << "Startup failed: "
            << dispatcher::common::to_string(startup_mode.error().code)
            << " | "
            << startup_mode.error().message
            << '\n';

        return EXIT_FAILURE;
    }

    const auto startup_envelope = dispatcher::contracts::make_success_envelope(
        correlation_id.str(),
        "Dispatcher server startup initialized"
    );
    const auto modules = dispatcher::core::get_initial_module_list();

    std::cout << version.product_name << " server" << '\n';
    std::cout << "Executable: " << version.executable_name << '\n';
    std::cout << "Version: " << version.version << '\n';
    std::cout << "Mode: " << startup_mode.value() << '\n';
    std::cout << "Status: starting" << '\n';
    std::cout << "CorrelationId: " << correlation_id.str() << '\n';
    std::cout << "Startup time UTC: " << dispatcher::common::to_iso8601_utc(startup_time) << '\n';
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