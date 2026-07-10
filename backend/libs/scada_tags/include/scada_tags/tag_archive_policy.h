#pragma once

#include <string_view>

namespace dispatcher::tags
{
    enum class TagArchivePolicy
    {
        Disabled,
        OnChange,
        Periodic,
        OnChangeWithDeadband,
        OnQualityChange,
        OnAlarm,
        AlwaysButThrottled
    };

    [[nodiscard]] std::string_view to_string(TagArchivePolicy policy) noexcept;
}
