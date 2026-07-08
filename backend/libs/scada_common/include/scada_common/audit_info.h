#pragma once

#include "scada_common/timestamp.h"
#include "scada_common/entity_id.h"

#include <string>
#include <optional>

namespace dispatcher::common
{

    struct AuditInfo
    {
        Timestamp created_at;

        std::optional<UserId> created_by;


        Timestamp updated_at;

        std::optional<UserId> updated_by;


        std::string change_reason;


        static AuditInfo create(
            std::optional<UserId> user_id = std::nullopt,
            std::string reason = {}
        );


        void update(
            std::optional<UserId> user_id = std::nullopt,
            std::string reason = {}
        );
    };

}