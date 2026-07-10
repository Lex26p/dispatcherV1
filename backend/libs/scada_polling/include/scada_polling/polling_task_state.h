#pragma once

#include <string_view>

namespace dispatcher::polling
{
    enum class PollingTaskState
    {
        Disabled,
        Waiting,
        Due,
        Running,
        Succeeded,
        Failed,
        Skipped
    };

    [[nodiscard]] std::string_view to_string(
        PollingTaskState state
    ) noexcept;
}
