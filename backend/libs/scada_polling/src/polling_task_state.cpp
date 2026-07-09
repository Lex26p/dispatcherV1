#include "scada_polling/polling_task_state.h"

namespace dispatcher::polling
{
    std::string_view to_string(
        PollingTaskState state
    ) noexcept
    {
        switch (state)
        {
        case PollingTaskState::Disabled:
            return "Disabled";
        case PollingTaskState::Waiting:
            return "Waiting";
        case PollingTaskState::Due:
            return "Due";
        case PollingTaskState::Running:
            return "Running";
        case PollingTaskState::Succeeded:
            return "Succeeded";
        case PollingTaskState::Failed:
            return "Failed";
        case PollingTaskState::Skipped:
            return "Skipped";
        }

        return "Unknown";
    }
}