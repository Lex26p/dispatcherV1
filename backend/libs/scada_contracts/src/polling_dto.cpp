#include "scada_contracts/polling/polling_dto.h"

bool dispatcher::contracts::polling::PollingGroupExecutionResultDto::is_success() const noexcept
{
    return status == "Succeeded";
}

bool dispatcher::contracts::polling::PollingTaskValidationResultDto::is_valid() const noexcept
{
    return issues.empty();
}

bool dispatcher::contracts::polling::PollingGroupValidationResultDto::is_valid() const noexcept
{
    return issues.empty();
}