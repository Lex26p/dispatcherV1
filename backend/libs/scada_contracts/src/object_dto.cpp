#include "scada_contracts/objects/object_dto.h"

namespace dispatcher::contracts::objects
{
    bool ObjectTreeValidationResultDto::is_valid() const noexcept
    {
        return issues.empty();
    }

    bool ResponsibilityZoneValidationResultDto::is_valid() const noexcept
    {
        return issues.empty();
    }
}