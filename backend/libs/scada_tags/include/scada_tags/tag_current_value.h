#pragma once

#include "scada_tags/tag_ids.h"
#include "scada_tags/tag_quality.h"
#include "scada_tags/tag_value_type.h"

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace dispatcher::tags
{
    enum class TagValueSource
    {
        Unknown,
        Device,
        Manual,
        Calculation,
        System,
        Simulation,
        External
    };

    using TagTimestamp = std::chrono::system_clock::time_point;

    using TagValuePayload = std::variant<
        std::monostate,
        bool,
        std::int32_t,
        std::int64_t,
        float,
        double,
        std::string
    >;

    struct TagCurrentValue
    {
        TagId tag_id;

        TagValueType value_type = TagValueType::Double;
        TagQuality quality = TagQuality::NotInitialized;
        TagValueSource source = TagValueSource::Unknown;

        TagValuePayload raw_value;
        TagValuePayload engineering_value;

        TagTimestamp timestamp{};
        TagTimestamp source_timestamp{};
        TagTimestamp server_timestamp{};

        std::optional<TagValuePayload> last_good_value;
        std::optional<TagTimestamp> last_good_timestamp;

        std::uint64_t change_counter = 0;

        [[nodiscard]] bool has_value() const noexcept;
        [[nodiscard]] bool has_good_quality() const noexcept;
        [[nodiscard]] bool has_last_good_value() const noexcept;
        [[nodiscard]] bool is_initialized() const noexcept;
        [[nodiscard]] bool is_from_runtime_source() const noexcept;

        void mark_changed() noexcept;
    };

    [[nodiscard]] std::string_view to_string(
        TagValueSource source
    ) noexcept;
}
