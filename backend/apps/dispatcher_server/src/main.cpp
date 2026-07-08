#include "scada_common/error.h"
#include "scada_common/timestamp.h"
#include "scada_common/version.h"
#include "scada_contracts/api_envelope.h"
#include "scada_core/application_context.h"
#include "scada_core/event_bus.h"
#include "scada_core/module_info.h"

#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
    dispatcher::common::Result<void> register_initial_modules(dispatcher::core::ApplicationContext& context)
    {
        for (auto module : dispatcher::core::get_initial_module_list())
        {
            auto result = context.register_module(std::move(module));
            if (result.is_failure())
            {
                return result;
            }
        }

        return dispatcher::common::Result<void>::success();
    }

    void print_startup_error(const dispatcher::common::Error& error)
    {
        std::cerr << "Startup failed: "
            << dispatcher::common::to_string(error.code)
            << " | "
            << error.message
            << '\n';
    }
}

int main()
{
    const auto version = dispatcher::common::get_version_info();

    dispatcher::core::ApplicationContext context{
        std::string{version.product_name},
        std::string{version.executable_name}
    };

    const auto register_result = register_initial_modules(context);
    if (register_result.is_failure())
    {
        print_startup_error(register_result.error());
        return EXIT_FAILURE;
    }

    const auto start_result = context.start();
    if (start_result.is_failure())
    {
        print_startup_error(start_result.error());
        return EXIT_FAILURE;
    }

    const auto startup_time = dispatcher::common::now_utc();
    const auto startup_envelope = dispatcher::contracts::make_success_envelope(
        context.startup_correlation_id().str(),
        "Dispatcher application context started"
    );
    const auto startup_event = dispatcher::core::make_core_event(
        "ApplicationStarted",
        "Dispatcher server application context started",
        context.startup_correlation_id().str()
    );

    std::cout << context.application_name() << " server" << '\n';
    std::cout << "Executable: " << context.executable_name() << '\n';
    std::cout << "Version: " << version.version << '\n';
    std::cout << "Mode: " << version.mode << '\n';
    std::cout << "Status: " << (context.is_started() ? "started" : "not_started") << '\n';
    std::cout << "CorrelationId: " << context.startup_correlation_id().str() << '\n';
    std::cout << "Startup time UTC: " << dispatcher::common::to_iso8601_utc(startup_time) << '\n';
    std::cout << "Startup message: " << startup_envelope.message << '\n';
    std::cout << "Startup event: " << startup_event.type << " | " << startup_event.message << '\n';
    std::cout << "Modules:" << '\n';

    for (const auto& module : context.modules())
    {
        std::cout << "  - " << module.code
            << " | " << module.name
            << " | " << dispatcher::core::to_string(module.status)
            << '\n';
    }

    return EXIT_SUCCESS;
}