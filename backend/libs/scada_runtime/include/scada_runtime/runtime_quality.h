#pragma once

#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_quality.h"

namespace dispatcher::runtime
{
    [[nodiscard]] bool is_good_runtime_quality(
        dispatcher::tags::TagQuality quality
    ) noexcept;

    [[nodiscard]] bool is_bad_runtime_quality(
        dispatcher::tags::TagQuality quality
    ) noexcept;

    [[nodiscard]] bool can_update_last_good_value(
        dispatcher::tags::TagQuality quality,
        const dispatcher::tags::TagValuePayload& value
    ) noexcept;
}
