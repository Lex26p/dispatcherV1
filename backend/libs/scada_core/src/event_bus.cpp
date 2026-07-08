#include "scada_core/event_bus.h"

namespace dispatcher::core
{
    CoreEvent make_core_event(std::string_view type, std::string_view message, std::string_view correlation_id)
    {
        return CoreEvent{
            .type = std::string{type},
            .message = std::string{message},
            .correlation_id = std::string{correlation_id}
        };
    }
}