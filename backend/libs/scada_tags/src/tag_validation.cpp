#include "scada_tags/tag_validation.h"

#include <cmath>

namespace dispatcher::tags
{
    bool TagValidationResult::is_valid() const noexcept
    {
        return issues.empty();
    }

    bool requires_object(
        TagType type
    ) noexcept
    {
        switch (type)
        {
        case TagType::DeviceTag:
        case TagType::ManualTag:
        case TagType::CalculatedTag:
        case TagType::VirtualTag:
        case TagType::CommandTag:
        case TagType::SimulationTag:
        case TagType::ExternalTag:
            return true;

        case TagType::SystemTag:
            return false;
        }

        return true;
    }

    bool requires_device(
        TagType type
    ) noexcept
    {
        switch (type)
        {
        case TagType::DeviceTag:
        case TagType::CommandTag:
            return true;

        case TagType::ManualTag:
        case TagType::CalculatedTag:
        case TagType::SystemTag:
        case TagType::VirtualTag:
        case TagType::SimulationTag:
        case TagType::ExternalTag:
            return false;
        }

        return false;
    }

    bool requires_address(
        TagType type
    ) noexcept
    {
        switch (type)
        {
        case TagType::DeviceTag:
        case TagType::CommandTag:
        case TagType::ExternalTag:
            return true;

        case TagType::ManualTag:
        case TagType::CalculatedTag:
        case TagType::SystemTag:
        case TagType::VirtualTag:
        case TagType::SimulationTag:
            return false;
        }

        return false;
    }

    bool supports_extraction(
        TagValueType value_type
    ) noexcept
    {
        switch (value_type)
        {
        case TagValueType::Boolean:
        case TagValueType::Int32:
        case TagValueType::Int64:
        case TagValueType::Float:
        case TagValueType::Double:
        case TagValueType::Enum:
            return true;

        case TagValueType::String:
        case TagValueType::Json:
            return false;
        }

        return false;
    }

    bool is_valid_engineering_transform(
        double scale,
        double offset
    ) noexcept
    {
        return std::isfinite(scale) &&
            std::isfinite(offset) &&
            scale != 0.0;
    }

    TagValidationResult validate_tag(
        const Tag& tag
    )
    {
        TagValidationResult result;

        if (!tag.has_valid_identity())
        {
            result.issues.push_back(
                TagValidationIssue{
                    .code = TagValidationCode::EmptyIdentity,
                    .tag_id = tag.id,
                    .message = "Tag identity is incomplete. Required fields: id, code, name."
                }
            );
        }

        if (requires_object(tag.type) && !tag.is_assigned_to_object())
        {
            result.issues.push_back(
                TagValidationIssue{
                    .code = TagValidationCode::MissingObjectAssignment,
                    .tag_id = tag.id,
                    .message = "Tag must be assigned to object for selected tag type."
                }
            );
        }

        if (requires_device(tag.type) && !tag.is_assigned_to_device())
        {
            result.issues.push_back(
                TagValidationIssue{
                    .code = TagValidationCode::MissingDeviceAssignment,
                    .tag_id = tag.id,
                    .message = "Tag must be assigned to device for selected tag type."
                }
            );
        }

        if (requires_address(tag.type) &&
            !tag.address.has_address() &&
            !tag.address.has_protocol_payload())
        {
            result.issues.push_back(
                TagValidationIssue{
                    .code = TagValidationCode::MissingAddress,
                    .tag_id = tag.id,
                    .message = "Tag address or protocol address payload is required for selected tag type."
                }
            );
        }

        if (!is_valid_engineering_transform(tag.scale, tag.offset))
        {
            result.issues.push_back(
                TagValidationIssue{
                    .code = TagValidationCode::InvalidEngineeringTransform,
                    .tag_id = tag.id,
                    .message = "Tag engineering transform is invalid. Scale and offset must be finite, scale must not be zero."
                }
            );
        }

        if (tag.address.bit_index.has_value() && tag.address.bit_index.value() > 63)
        {
            result.issues.push_back(
                TagValidationIssue{
                    .code = TagValidationCode::InvalidBitIndex,
                    .tag_id = tag.id,
                    .message = "Tag bit index must be in range 0..63."
                }
            );
        }

        if (tag.address.bit_length.has_value() &&
            (tag.address.bit_length.value() == 0 || tag.address.bit_length.value() > 64))
        {
            result.issues.push_back(
                TagValidationIssue{
                    .code = TagValidationCode::InvalidBitLength,
                    .tag_id = tag.id,
                    .message = "Tag bit length must be in range 1..64."
                }
            );
        }

        if (tag.address.shift.has_value() && tag.address.shift.value() > 63)
        {
            result.issues.push_back(
                TagValidationIssue{
                    .code = TagValidationCode::InvalidShift,
                    .tag_id = tag.id,
                    .message = "Tag shift must be in range 0..63."
                }
            );
        }

        if (tag.address.uses_bit_extraction() && !supports_extraction(tag.value_type))
        {
            result.issues.push_back(
                TagValidationIssue{
                    .code = TagValidationCode::UnsupportedExtractionForValueType,
                    .tag_id = tag.id,
                    .message = "Tag bit extraction is not supported for selected value type."
                }
            );
        }

        return result;
    }

    std::string_view to_string(
        TagValidationCode code
    ) noexcept
    {
        switch (code)
        {
        case TagValidationCode::EmptyIdentity:
            return "EmptyIdentity";
        case TagValidationCode::MissingObjectAssignment:
            return "MissingObjectAssignment";
        case TagValidationCode::MissingDeviceAssignment:
            return "MissingDeviceAssignment";
        case TagValidationCode::MissingAddress:
            return "MissingAddress";
        case TagValidationCode::InvalidEngineeringTransform:
            return "InvalidEngineeringTransform";
        case TagValidationCode::InvalidBitIndex:
            return "InvalidBitIndex";
        case TagValidationCode::InvalidBitLength:
            return "InvalidBitLength";
        case TagValidationCode::InvalidShift:
            return "InvalidShift";
        case TagValidationCode::UnsupportedExtractionForValueType:
            return "UnsupportedExtractionForValueType";
        }

        return "Unknown";
    }
}
