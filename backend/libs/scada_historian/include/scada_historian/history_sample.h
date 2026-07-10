#pragma once

#include "scada_historian/history_ids.h"
#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"
#include "scada_tags/tag_quality.h"
#include "scada_tags/tag_value_type.h"

#include <cstdint>
#include <string>

namespace dispatcher::historian
{
    struct HistorySample
    {
        HistorySampleId id;

        dispatcher::tags::TagId tag_id;

        dispatcher::tags::TagValueType value_type = dispatcher::tags::TagValueType::Double;
        dispatcher::tags::TagQuality quality = dispatcher::tags::TagQuality::NotInitialized;
        dispatcher::tags::TagValueSource source = dispatcher::tags::TagValueSource::Unknown;

        dispatcher::tags::TagValuePayload raw_value;
        dispatcher::tags::TagValuePayload engineering_value;

        dispatcher::tags::TagTimestamp timestamp{};
        dispatcher::tags::TagTimestamp source_timestamp{};
        dispatcher::tags::TagTimestamp server_timestamp{};

        std::uint64_t change_counter = 0;

        std::string engineering_unit;
        std::string message;

        [[nodiscard]] bool has_sample_id() const noexcept;
        [[nodiscard]] bool has_tag_id() const noexcept;
        [[nodiscard]] bool has_value() const noexcept;
        [[nodiscard]] bool has_good_quality() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
        [[nodiscard]] bool has_valid_identity() const noexcept;
    };

    [[nodiscard]] HistorySample make_history_sample_from_current_value(
        const dispatcher::tags::TagCurrentValue& current_value,
        std::string engineering_unit = {},
        std::string message = {}
    );
}
