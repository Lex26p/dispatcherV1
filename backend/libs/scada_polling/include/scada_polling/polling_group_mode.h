#pragma once

#include <string_view>

namespace dispatcher::polling
{
    enum class PollingGroupMode
    {
        Sequential,
        Batch
    };

    [[nodiscard]] std::string_view to_string(
        PollingGroupMode mode
    ) noexcept;
}
