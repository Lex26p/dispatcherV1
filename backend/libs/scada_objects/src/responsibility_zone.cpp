#include "scada_objects/responsibility_zone.h"

namespace dispatcher::objects
{
    namespace
    {
        [[nodiscard]] bool contains_object_id(
            const std::vector<ObjectId>& values,
            const ObjectId& value
        )
        {
            for (const auto& existing_value : values)
            {
                if (existing_value == value)
                {
                    return true;
                }
            }

            return false;
        }

        [[nodiscard]] bool contains_uuid(
            const std::vector<dispatcher::common::Uuid>& values,
            const dispatcher::common::Uuid& value
        )
        {
            for (const auto& existing_value : values)
            {
                if (existing_value == value)
                {
                    return true;
                }
            }

            return false;
        }

        [[nodiscard]] bool contains_string(
            const std::vector<std::string>& values,
            std::string_view value
        )
        {
            for (const auto& existing_value : values)
            {
                if (existing_value == value)
                {
                    return true;
                }
            }

            return false;
        }

        [[nodiscard]] bool has_duplicate_object_id(
            const std::vector<ObjectId>& values
        )
        {
            for (std::size_t left_index = 0; left_index < values.size(); ++left_index)
            {
                if (values[left_index].empty())
                {
                    continue;
                }

                for (std::size_t right_index = left_index + 1; right_index < values.size(); ++right_index)
                {
                    if (values[left_index] == values[right_index])
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        [[nodiscard]] bool has_duplicate_uuid(
            const std::vector<dispatcher::common::Uuid>& values
        )
        {
            for (std::size_t left_index = 0; left_index < values.size(); ++left_index)
            {
                if (values[left_index].empty())
                {
                    continue;
                }

                for (std::size_t right_index = left_index + 1; right_index < values.size(); ++right_index)
                {
                    if (values[left_index] == values[right_index])
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        [[nodiscard]] bool has_duplicate_string(
            const std::vector<std::string>& values
        )
        {
            for (std::size_t left_index = 0; left_index < values.size(); ++left_index)
            {
                if (values[left_index].empty())
                {
                    continue;
                }

                for (std::size_t right_index = left_index + 1; right_index < values.size(); ++right_index)
                {
                    if (values[left_index] == values[right_index])
                    {
                        return true;
                    }
                }
            }

            return false;
        }
    }

    bool ResponsibilityZone::has_valid_identity() const noexcept
    {
        return !id.empty() && !code.empty() && !name.empty();
    }

    bool ResponsibilityZone::contains_object(
        const ObjectId& object_id
    ) const
    {
        return contains_object_id(object_ids, object_id);
    }

    bool ResponsibilityZone::contains_user(
        const dispatcher::common::Uuid& user_id
    ) const
    {
        return contains_uuid(user_ids, user_id);
    }

    bool ResponsibilityZone::contains_role_code(
        std::string_view role_code
    ) const
    {
        return contains_string(role_codes, role_code);
    }

    bool ResponsibilityZoneValidationResult::is_valid() const noexcept
    {
        return issues.empty();
    }

    ResponsibilityZoneValidationResult validate_responsibility_zone(
        const ResponsibilityZone& zone
    )
    {
        ResponsibilityZoneValidationResult result;

        if (!zone.has_valid_identity())
        {
            result.issues.push_back(
                ResponsibilityZoneValidationIssue{
                    .code = ResponsibilityZoneValidationCode::EmptyIdentity,
                    .zone_id = zone.id,
                    .message = "Responsibility zone identity is incomplete. Required fields: id, code, name."
                }
            );
        }

        if (zone.object_ids.empty())
        {
            result.issues.push_back(
                ResponsibilityZoneValidationIssue{
                    .code = ResponsibilityZoneValidationCode::EmptyObjectList,
                    .zone_id = zone.id,
                    .message = "Responsibility zone must contain at least one object."
                }
            );
        }

        if (has_duplicate_object_id(zone.object_ids))
        {
            result.issues.push_back(
                ResponsibilityZoneValidationIssue{
                    .code = ResponsibilityZoneValidationCode::DuplicateObjectId,
                    .zone_id = zone.id,
                    .message = "Responsibility zone contains duplicate object id."
                }
            );
        }

        if (has_duplicate_uuid(zone.user_ids))
        {
            result.issues.push_back(
                ResponsibilityZoneValidationIssue{
                    .code = ResponsibilityZoneValidationCode::DuplicateUserId,
                    .zone_id = zone.id,
                    .message = "Responsibility zone contains duplicate user id."
                }
            );
        }

        if (has_duplicate_string(zone.role_codes))
        {
            result.issues.push_back(
                ResponsibilityZoneValidationIssue{
                    .code = ResponsibilityZoneValidationCode::DuplicateRoleCode,
                    .zone_id = zone.id,
                    .message = "Responsibility zone contains duplicate role code."
                }
            );
        }

        return result;
    }

    std::string_view to_string(
        ResponsibilityZoneValidationCode code
    ) noexcept
    {
        switch (code)
        {
        case ResponsibilityZoneValidationCode::EmptyIdentity:
            return "EmptyIdentity";
        case ResponsibilityZoneValidationCode::EmptyObjectList:
            return "EmptyObjectList";
        case ResponsibilityZoneValidationCode::DuplicateObjectId:
            return "DuplicateObjectId";
        case ResponsibilityZoneValidationCode::DuplicateUserId:
            return "DuplicateUserId";
        case ResponsibilityZoneValidationCode::DuplicateRoleCode:
            return "DuplicateRoleCode";
        }

        return "Unknown";
    }
}