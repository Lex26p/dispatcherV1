#include "scada_core/application_context.h"

#include "scada_common/error.h"

#include <algorithm>
#include <utility>

namespace dispatcher::core
{
    ApplicationContext::ApplicationContext(std::string application_name, std::string executable_name)
        : application_name_(std::move(application_name)),
        executable_name_(std::move(executable_name)),
        startup_correlation_id_(dispatcher::common::CorrelationId::startup())
    {
    }

    const std::string& ApplicationContext::application_name() const noexcept
    {
        return application_name_;
    }

    const std::string& ApplicationContext::executable_name() const noexcept
    {
        return executable_name_;
    }

    const dispatcher::common::CorrelationId& ApplicationContext::startup_correlation_id() const noexcept
    {
        return startup_correlation_id_;
    }

    dispatcher::common::Result<void> ApplicationContext::register_module(ModuleInfo module)
    {
        if (module.code.empty())
        {
            return dispatcher::common::Result<void>::failure(
                dispatcher::common::make_error(
                    dispatcher::common::ErrorCode::ValidationFailed,
                    "Module code must not be empty"
                )
            );
        }

        const auto duplicate = std::ranges::find_if(
            modules_,
            [&module](const ModuleInfo& existing)
            {
                return existing.code == module.code;
            }
        );

        if (duplicate != modules_.end())
        {
            return dispatcher::common::Result<void>::failure(
                dispatcher::common::make_error(
                    dispatcher::common::ErrorCode::AlreadyExists,
                    "Module already registered"
                )
            );
        }

        modules_.push_back(std::move(module));
        return dispatcher::common::Result<void>::success();
    }

    dispatcher::common::Result<void> ApplicationContext::start()
    {
        if (application_name_.empty())
        {
            return dispatcher::common::Result<void>::failure(
                dispatcher::common::make_error(
                    dispatcher::common::ErrorCode::ValidationFailed,
                    "Application name must not be empty"
                )
            );
        }

        if (executable_name_.empty())
        {
            return dispatcher::common::Result<void>::failure(
                dispatcher::common::make_error(
                    dispatcher::common::ErrorCode::ValidationFailed,
                    "Executable name must not be empty"
                )
            );
        }

        started_ = true;
        return dispatcher::common::Result<void>::success();
    }

    bool ApplicationContext::is_started() const noexcept
    {
        return started_;
    }

    const std::vector<ModuleInfo>& ApplicationContext::modules() const noexcept
    {
        return modules_;
    }
}
