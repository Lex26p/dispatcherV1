#include "scada_historian/history_sample.h"

#include <utility>
#include <variant>

namespace dispatcher::historian
{
    bool HistorySample::has_sample_id() const noexcept
    {
        return !id.empty();
    }

    bool HistorySample::has_tag_id() const noexcept
    {
        return !tag_id.empty();
    }

    bool HistorySample::has_value() const noexcept
    {
        return !std::holds_alternative<std::monostate>(engineering_value);
    }

    bool HistorySample::has_good_quality() const noexcept
    {
        return quality == dispatcher::tags::TagQuality::Good ||
            quality == dispatcher::tags::TagQuality::Manual ||
            quality == dispatcher::tags::TagQuality::Simulation;
    }

    bool HistorySample::has_message() const noexcept
    {
        return !message.empty();
    }

    bool HistorySample::has_valid_identity() const noexcept
    {
        return has_tag_id();
    }

    HistorySample make_history_sample_from_current_value(
        const dispatcher::tags::TagCurrentValue& current_value,
        std::string engineering_unit,
        std::string message
    )
    {
        HistorySample sample;

        sample.tag_id = current_value.tag_id;
        sample.value_type = current_value.value_type;
        sample.quality = current_value.quality;
        sample.source = current_value.source;

        sample.raw_value = current_value.raw_value;
        sample.engineering_value = current_value.engineering_value;

        sample.timestamp = current_value.timestamp;
        sample.source_timestamp = current_value.source_timestamp;
        sample.server_timestamp = current_value.server_timestamp;

        sample.change_counter = current_value.change_counter;

        sample.engineering_unit = std::move(engineering_unit);
        sample.message = std::move(message);

        return sample;
    }
}
