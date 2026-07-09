#pragma once

#include "scada_tags/tag.h"
#include "scada_tags/tag_type.h"
#include "scada_tags/tag_value_type.h"

#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::tags
{
    enum class TagValidationCode
    {
        EmptyIdentity,
        MissingObjectAssignment,
        MissingDeviceAssignment,
        MissingAddress,
        InvalidEngineeringTransform,
        InvalidBitIndex,
        InvalidBitLength,
        InvalidShift,
        UnsupportedExtractionForValueType
    };

    struct TagValidationIssue
    {
        TagValidationCode code = TagValidationCode::EmptyIdentity;
        TagId tag_id;
        std::string message;
    };

    struct TagValidationResult
    {
        std::vector<TagValidationIssue> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };

    [[nodiscard]] bool requires_object(
        TagType type
    ) noexcept;

    [[nodiscard]] bool requires_device(
        TagType type
    ) noexcept;

    [[nodiscard]] bool requires_address(
        TagType type
    ) noexcept;

    [[nodiscard]] bool supports_extraction(
        TagValueType value_type
    ) noexcept;

    [[nodiscard]] bool is_valid_engineering_transform(
        double scale,
        double offset
    ) noexcept;

    [[nodiscard]] TagValidationResult validate_tag(
        const Tag& tag
    );

    [[nodiscard]] std::string_view to_string(
        TagValidationCode code
    ) noexcept;
}