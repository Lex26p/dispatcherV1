#pragma once

#include "scada_common/result.h"

#include <string>
#include <string_view>

namespace dispatcher::core
{
    struct CoreEvent
    {
        std::string type;
        std::string message;
        std::string correlation_id;
    };

    class IEventBus
    {
    public:
        virtual ~IEventBus() = default;

        virtual dispatcher::common::Result<void> publish(const CoreEvent& event) = 0;
    };

    CoreEvent make_core_event(std::string_view type, std::string_view message, std::string_view correlation_id);
}