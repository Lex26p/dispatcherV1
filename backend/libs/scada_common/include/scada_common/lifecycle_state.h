#pragma once

#include <string_view>

namespace dispatcher::common
{

    enum class LifecycleState
    {
        Active,

        Disabled,

        Archived,

        Deleted
    };


    [[nodiscard]]
    std::string_view to_string(LifecycleState state) noexcept;


    [[nodiscard]]
    LifecycleState lifecycle_state_from_string(std::string_view value) noexcept;

}