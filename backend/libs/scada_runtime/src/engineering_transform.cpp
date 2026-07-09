#include "scada_runtime/engineering_transform.h"

#include "scada_runtime/runtime_value_conversion.h"

#include <cmath>

namespace dispatcher::runtime
{
    bool RuntimeEngineeringTransformResult::is_success() const noexcept
    {
        return success;
    }

    bool RuntimeEngineeringTransformResult::was_transformed() const noexcept
    {
        return transformed;
    }

    bool RuntimeEngineeringTransformResult::has_message() const noexcept
    {
        return !message.empty();
    }

    bool can_apply_engineering_transform(
        const dispatcher::tags::TagValuePayload& value
    ) noexcept
    {
        return is_numeric_runtime_value(value);
    }

    RuntimeEngineeringTransformResult apply_engineering_transform(
        const dispatcher::tags::TagValuePayload& value,
        RuntimeEngineeringTransformOptions options
    )
    {
        RuntimeEngineeringTransformResult result;
        result.raw_value = value;
        result.engineering_value = value;

        if (!options.enabled)
        {
            result.success = true;
            result.transformed = false;
            result.message = "Engineering transform is disabled.";
            return result;
        }

        if (!std::isfinite(options.scale) ||
            !std::isfinite(options.offset) ||
            options.scale == 0.0)
        {
            result.success = false;
            result.transformed = false;
            result.message = "Engineering transform options are invalid.";
            return result;
        }

        const auto numeric = runtime_value_to_double(value);

        if (!numeric.has_value())
        {
            result.success = true;
            result.transformed = false;
            result.message = "Engineering transform skipped for non-numeric value.";
            return result;
        }

        result.engineering_value = numeric.value() * options.scale + options.offset;
        result.success = true;
        result.transformed = true;
        result.message = "Engineering transform applied.";

        return result;
    }
}