#include "scada_polling/polling_group_mode.h"

namespace dispatcher::polling
{
    std::string_view to_string(
        PollingGroupMode mode
    ) noexcept
    {
        switch (mode)
        {
        case PollingGroupMode::Sequential:
            return "Sequential";
        case PollingGroupMode::Batch:
            return "Batch";
        }

        return "Unknown";
    }
}