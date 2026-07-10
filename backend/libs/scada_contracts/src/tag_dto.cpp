#include "scada_contracts/tags/tag_dto.h"

namespace dispatcher::contracts::tags
{
    bool TagValidationResultDto::is_valid() const noexcept
    {
        return issues.empty();
    }
}
