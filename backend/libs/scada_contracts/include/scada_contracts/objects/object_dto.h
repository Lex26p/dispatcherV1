#pragma once

#include <optional>
#include <string>
#include <vector>

namespace dispatcher::contracts::objects
{
    struct ObjectNodeDto
    {
        std::string id;
        std::optional<std::string> parent_id;
        std::string type;
        std::string code;
        std::string name;
        std::string description;
        std::string path;
    };

    struct ObjectTreeDto
    {
        std::vector<ObjectNodeDto> objects;
    };

    struct ObjectTreeValidationIssueDto
    {
        std::string code;
        std::string object_id;
        std::string message;
    };

    struct ObjectTreeValidationResultDto
    {
        std::vector<ObjectTreeValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };

    struct ResponsibilityZoneDto
    {
        std::string id;
        std::string code;
        std::string name;
        std::string description;

        std::vector<std::string> object_ids;
        std::vector<std::string> user_ids;
        std::vector<std::string> role_codes;
    };

    struct ResponsibilityZoneValidationIssueDto
    {
        std::string code;
        std::string zone_id;
        std::string message;
    };

    struct ResponsibilityZoneValidationResultDto
    {
        std::vector<ResponsibilityZoneValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };
}