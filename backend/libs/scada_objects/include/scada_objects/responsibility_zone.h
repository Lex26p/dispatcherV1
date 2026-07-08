#pragma once

#include "scada_common/id_types.h"

#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::objects
{
    struct ResponsibilityZone
    {
        dispatcher::common::Uuid id;
        std::string code;
        std::string name;
        std::string description;

        std::vector<dispatcher::common::Uuid> object_ids;
        std::vector<dispatcher::common::Uuid> user_ids;
        std::vector<std::string> role_codes;

        [[nodiscard]] bool has_valid_identity() const noexcept;

        [[nodiscard]] bool contains_object(
            const dispatcher::common::Uuid& object_id
        ) const;

        [[nodiscard]] bool contains_user(
            const dispatcher::common::Uuid& user_id
        ) const;

        [[nodiscard]] bool contains_role_code(
            std::string_view role_code
        ) const;
    };

    enum class ResponsibilityZoneValidationCode
    {
        EmptyIdentity,
        EmptyObjectList,
        DuplicateObjectId,
        DuplicateUserId,
        DuplicateRoleCode
    };

    struct ResponsibilityZoneValidationIssue
    {
        ResponsibilityZoneValidationCode code = ResponsibilityZoneValidationCode::EmptyIdentity;
        dispatcher::common::Uuid zone_id;
        std::string message;
    };

    struct ResponsibilityZoneValidationResult
    {
        std::vector<ResponsibilityZoneValidationIssue> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };

    [[nodiscard]] ResponsibilityZoneValidationResult validate_responsibility_zone(
        const ResponsibilityZone& zone
    );

    [[nodiscard]] std::string_view to_string(
        ResponsibilityZoneValidationCode code
    ) noexcept;
}