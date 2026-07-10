#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::contracts::runtime
{
    struct RuntimeValueDto
    {
        std::string tag_id;

        std::string value_type;
        std::string quality;
        std::string source;

        std::string raw_value;
        std::string engineering_value;

        std::string timestamp;
        std::string source_timestamp;
        std::string server_timestamp;

        std::string last_good_value;
        std::string last_good_timestamp;

        std::uint64_t change_counter = 0;

        [[nodiscard]] bool has_value() const noexcept;
        [[nodiscard]] bool has_last_good_value() const noexcept;
    };

    struct RuntimeValueEventDto
    {
        std::string tag_id;

        std::string event_type;
        std::string change_kind;

        std::string previous_quality;
        std::string current_quality;

        std::string timestamp;

        std::uint64_t change_counter = 0;

        std::string message;

        [[nodiscard]] bool has_message() const noexcept;
    };

    struct RuntimeValueApplyResultDto
    {
        std::string tag_id;

        bool success = false;
        bool inserted = false;
        bool updated = false;
        bool changed = false;
        bool good_quality = false;
        bool last_good_updated = false;
        bool value_converted = false;
        bool engineering_transform_applied = false;

        std::string change_kind;

        std::optional<RuntimeValueEventDto> event;

        std::uint64_t previous_change_counter = 0;
        std::uint64_t new_change_counter = 0;

        std::string quality;
        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_changed() const noexcept;
        [[nodiscard]] bool has_event() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    struct RuntimeValueSnapshotDto
    {
        RuntimeValueDto value;

        std::string saved_at;
        std::string updated_at;
    };

    struct RuntimeValueValidationIssueDto
    {
        std::string code;
        std::string tag_id;
        std::string message;
    };

    struct RuntimeValueValidationResultDto
    {
        std::vector<RuntimeValueValidationIssueDto> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };
}
