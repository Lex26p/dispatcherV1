#include "scada_contracts/runtime/runtime_dto.h"

namespace dispatcher::contracts::runtime
{
    bool RuntimeValueDto::has_value() const noexcept
    {
        return !engineering_value.empty();
    }

    bool RuntimeValueDto::has_last_good_value() const noexcept
    {
        return !last_good_value.empty();
    }

    bool RuntimeValueEventDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool RuntimeValueApplyResultDto::is_success() const noexcept
    {
        return success;
    }

    bool RuntimeValueApplyResultDto::has_changed() const noexcept
    {
        return changed;
    }

    bool RuntimeValueApplyResultDto::has_event() const noexcept
    {
        return event.has_value();
    }

    bool RuntimeValueApplyResultDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool RuntimeValueValidationResultDto::is_valid() const noexcept
    {
        return issues.empty();
    }
}
