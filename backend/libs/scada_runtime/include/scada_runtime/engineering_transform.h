#pragma once

#include "scada_tags/tag_current_value.h"

#include <string>

namespace dispatcher::runtime
{
    struct RuntimeEngineeringTransformOptions
    {
        bool enabled = true;

        double scale = 1.0;
        double offset = 0.0;
    };

    struct RuntimeEngineeringTransformResult
    {
        dispatcher::tags::TagValuePayload raw_value;
        dispatcher::tags::TagValuePayload engineering_value;

        bool success = false;
        bool transformed = false;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool was_transformed() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    [[nodiscard]] bool can_apply_engineering_transform(
        const dispatcher::tags::TagValuePayload& value
    ) noexcept;

    [[nodiscard]] RuntimeEngineeringTransformResult apply_engineering_transform(
        const dispatcher::tags::TagValuePayload& value,
        RuntimeEngineeringTransformOptions options
    );
}
