#pragma once

#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_value_type.h"

#include <optional>
#include <string>

namespace dispatcher::runtime
{
    struct RuntimeValueConversionResult
    {
        dispatcher::tags::TagValuePayload value;
        dispatcher::tags::TagValueType value_type = dispatcher::tags::TagValueType::Double;

        bool success = false;
        bool converted = false;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool was_converted() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    [[nodiscard]] dispatcher::tags::TagValueType infer_runtime_value_type(
        const dispatcher::tags::TagValuePayload& value,
        dispatcher::tags::TagValueType fallback
    ) noexcept;

    [[nodiscard]] bool is_numeric_runtime_value(
        const dispatcher::tags::TagValuePayload& value
    ) noexcept;

    [[nodiscard]] std::optional<double> runtime_value_to_double(
        const dispatcher::tags::TagValuePayload& value
    );

    [[nodiscard]] std::string runtime_value_to_string(
        const dispatcher::tags::TagValuePayload& value
    );

    [[nodiscard]] RuntimeValueConversionResult convert_runtime_value(
        const dispatcher::tags::TagValuePayload& value,
        dispatcher::tags::TagValueType target_type
    );
}
