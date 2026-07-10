#include "scada_tags/tag_archive_policy.h"

namespace dispatcher::tags
{
    std::string_view to_string(TagArchivePolicy policy) noexcept
    {
        switch (policy)
        {
        case TagArchivePolicy::Disabled:
            return "Disabled";
        case TagArchivePolicy::OnChange:
            return "OnChange";
        case TagArchivePolicy::Periodic:
            return "Periodic";
        case TagArchivePolicy::OnChangeWithDeadband:
            return "OnChangeWithDeadband";
        case TagArchivePolicy::OnQualityChange:
            return "OnQualityChange";
        case TagArchivePolicy::OnAlarm:
            return "OnAlarm";
        case TagArchivePolicy::AlwaysButThrottled:
            return "AlwaysButThrottled";
        }

        return "Unknown";
    }
}
