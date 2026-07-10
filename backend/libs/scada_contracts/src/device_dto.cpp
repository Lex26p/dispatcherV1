#include "scada_contracts/devices/device_dto.h"

namespace dispatcher::contracts::devices
{
    bool DeviceValidationResultDto::is_valid() const noexcept
    {
        return issues.empty();
    }
}
