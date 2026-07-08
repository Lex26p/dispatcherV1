#include "scada_common/audit_info.h"

#include <chrono>
#include <utility>

namespace dispatcher::common
{

    AuditInfo AuditInfo::create(
        std::optional<UserId> user_id,
        std::string reason
    )
    {
        AuditInfo info;

        const auto now = std::chrono::system_clock::now();

        info.created_at = now;

        info.created_by = user_id;

        info.updated_at = now;

        info.updated_by = user_id;

        info.change_reason = std::move(reason);

        return info;
    }


    void AuditInfo::update(
        std::optional<UserId> user_id,
        std::string reason
    )
    {
        updated_at = std::chrono::system_clock::now();

        updated_by = user_id;

        change_reason = std::move(reason);
    }

}