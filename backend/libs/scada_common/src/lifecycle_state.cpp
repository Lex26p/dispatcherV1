#include "scada_common/lifecycle_state.h"

namespace dispatcher::common
{

    std::string_view to_string(LifecycleState state) noexcept
    {
        switch (state)
        {
        case LifecycleState::Active:
            return "ACTIVE";

        case LifecycleState::Disabled:
            return "DISABLED";

        case LifecycleState::Archived:
            return "ARCHIVED";

        case LifecycleState::Deleted:
            return "DELETED";
        }

        return "UNKNOWN";
    }


    LifecycleState lifecycle_state_from_string(std::string_view value) noexcept
    {
        if (value == "ACTIVE")
        {
            return LifecycleState::Active;
        }

        if (value == "DISABLED")
        {
            return LifecycleState::Disabled;
        }

        if (value == "ARCHIVED")
        {
            return LifecycleState::Archived;
        }

        if (value == "DELETED")
        {
            return LifecycleState::Deleted;
        }

        return LifecycleState::Disabled;
    }

}