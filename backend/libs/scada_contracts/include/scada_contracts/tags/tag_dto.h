#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::contracts::tags
{
    struct TagAddressDto
    {
        std::string protocol;
        std::string address;
        std::string protocol_address_json;

        std::optional<std::uint16_t> bit_index;
        std::optional<std::uint32_t> byte_offset;
        std::optional<std::uint64_t> mask;
        std::optional<std::uint16_t> shift;
        std::optional<std::uint16_t> bit_length;
    };

    struct TagDto
    {
        std::string id;

        std::optional<std::string> object_id;
        std::optional<std::string> device_id;

        std::string type;
        std::string value_type;
        std::string archive_policy;

        std::string code;
        std::string name;
        std::string description;

        std::string engineering_unit;
        double scale = 1.0;
        double offset = 0.0;

        TagAddressDto address;

        bool is_enabled = true;
    };

    struct TagListDto
    {
        std::vector<TagDto> tags;
    };

    struct TagCurrentValueDto
    {
        std::string tag_id;

        std::string value_type;
        std::string quality;
        std::string source;

        std::optional<std::string> raw_value;
        std::optional<std::string> engineering_value;

        std::string timestamp;
        std::string source_timestamp;
        std::string server_timestamp;

        std::optional<std::string> last_good_value;
        std::string last_good_timestamp;

        std::uint64_t change_counter = 0;
    };

    struct TagValidationIssueDto
    {
        std::string code;
        std::string tag_id;
        std::string message;
    };

    struct TagValidationResultDto
    {
        std::vector<TagValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };
}