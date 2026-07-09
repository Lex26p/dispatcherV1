#include "scada_runtime/runtime_value_conversion.h"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

namespace dispatcher::runtime
{
    namespace
    {
        [[nodiscard]] std::string to_lower_copy(
            std::string value
        )
        {
            for (auto& ch : value)
            {
                if (ch >= 'A' && ch <= 'Z')
                {
                    ch = static_cast<char>(ch - 'A' + 'a');
                }
            }

            return value;
        }

        [[nodiscard]] std::optional<bool> string_to_bool(
            const std::string& value
        )
        {
            const auto normalized = to_lower_copy(value);

            if (normalized == "true" || normalized == "1" || normalized == "yes" || normalized == "on")
            {
                return true;
            }

            if (normalized == "false" || normalized == "0" || normalized == "no" || normalized == "off")
            {
                return false;
            }

            return std::nullopt;
        }

        [[nodiscard]] std::optional<double> string_to_double(
            const std::string& value
        )
        {
            try
            {
                std::size_t parsed = 0;
                const auto result = std::stod(value, &parsed);

                if (parsed != value.size())
                {
                    return std::nullopt;
                }

                return result;
            }
            catch (const std::exception&)
            {
                return std::nullopt;
            }
        }
    }

    bool RuntimeValueConversionResult::is_success() const noexcept
    {
        return success;
    }

    bool RuntimeValueConversionResult::was_converted() const noexcept
    {
        return converted;
    }

    bool RuntimeValueConversionResult::has_message() const noexcept
    {
        return !message.empty();
    }

    dispatcher::tags::TagValueType infer_runtime_value_type(
        const dispatcher::tags::TagValuePayload& value,
        dispatcher::tags::TagValueType fallback
    ) noexcept
    {
        if (std::holds_alternative<bool>(value))
        {
            return dispatcher::tags::TagValueType::Boolean;
        }

        if (std::holds_alternative<std::int32_t>(value))
        {
            return dispatcher::tags::TagValueType::Int32;
        }

        if (std::holds_alternative<std::int64_t>(value))
        {
            return dispatcher::tags::TagValueType::Int64;
        }

        if (std::holds_alternative<float>(value))
        {
            return dispatcher::tags::TagValueType::Float;
        }

        if (std::holds_alternative<double>(value))
        {
            return dispatcher::tags::TagValueType::Double;
        }

        if (std::holds_alternative<std::string>(value))
        {
            return dispatcher::tags::TagValueType::String;
        }

        return fallback;
    }

    bool is_numeric_runtime_value(
        const dispatcher::tags::TagValuePayload& value
    ) noexcept
    {
        return std::holds_alternative<std::int32_t>(value) ||
            std::holds_alternative<std::int64_t>(value) ||
            std::holds_alternative<float>(value) ||
            std::holds_alternative<double>(value);
    }

    std::optional<double> runtime_value_to_double(
        const dispatcher::tags::TagValuePayload& value
    )
    {
        if (std::holds_alternative<std::int32_t>(value))
        {
            return static_cast<double>(std::get<std::int32_t>(value));
        }

        if (std::holds_alternative<std::int64_t>(value))
        {
            return static_cast<double>(std::get<std::int64_t>(value));
        }

        if (std::holds_alternative<float>(value))
        {
            return static_cast<double>(std::get<float>(value));
        }

        if (std::holds_alternative<double>(value))
        {
            return std::get<double>(value);
        }

        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value) ? 1.0 : 0.0;
        }

        if (std::holds_alternative<std::string>(value))
        {
            return string_to_double(std::get<std::string>(value));
        }

        return std::nullopt;
    }

    std::string runtime_value_to_string(
        const dispatcher::tags::TagValuePayload& value
    )
    {
        if (std::holds_alternative<std::monostate>(value))
        {
            return {};
        }

        if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value) ? "true" : "false";
        }

        if (std::holds_alternative<std::int32_t>(value))
        {
            return std::to_string(std::get<std::int32_t>(value));
        }

        if (std::holds_alternative<std::int64_t>(value))
        {
            return std::to_string(std::get<std::int64_t>(value));
        }

        if (std::holds_alternative<float>(value))
        {
            return std::to_string(std::get<float>(value));
        }

        if (std::holds_alternative<double>(value))
        {
            return std::to_string(std::get<double>(value));
        }

        if (std::holds_alternative<std::string>(value))
        {
            return std::get<std::string>(value);
        }

        return {};
    }

    RuntimeValueConversionResult convert_runtime_value(
        const dispatcher::tags::TagValuePayload& value,
        dispatcher::tags::TagValueType target_type
    )
    {
        RuntimeValueConversionResult result;
        result.value_type = target_type;

        if (std::holds_alternative<std::monostate>(value))
        {
            result.success = false;
            result.value = value;
            result.message = "Runtime value is empty.";
            return result;
        }

        const auto current_type = infer_runtime_value_type(
            value,
            target_type
        );

        if (current_type == target_type)
        {
            result.success = true;
            result.converted = false;
            result.value = value;
            result.message = "Runtime value already has requested type.";
            return result;
        }

        switch (target_type)
        {
        case dispatcher::tags::TagValueType::Boolean:
        {
            if (std::holds_alternative<bool>(value))
            {
                result.value = std::get<bool>(value);
                result.success = true;
                return result;
            }

            if (const auto numeric = runtime_value_to_double(value); numeric.has_value())
            {
                result.value = numeric.value() != 0.0;
                result.success = true;
                result.converted = true;
                return result;
            }

            if (std::holds_alternative<std::string>(value))
            {
                const auto parsed = string_to_bool(std::get<std::string>(value));

                if (parsed.has_value())
                {
                    result.value = parsed.value();
                    result.success = true;
                    result.converted = true;
                    return result;
                }
            }

            break;
        }

        case dispatcher::tags::TagValueType::Int32:
        {
            if (const auto numeric = runtime_value_to_double(value); numeric.has_value())
            {
                result.value = static_cast<std::int32_t>(numeric.value());
                result.success = true;
                result.converted = true;
                return result;
            }

            break;
        }

        case dispatcher::tags::TagValueType::Int64:
        {
            if (const auto numeric = runtime_value_to_double(value); numeric.has_value())
            {
                result.value = static_cast<std::int64_t>(numeric.value());
                result.success = true;
                result.converted = true;
                return result;
            }

            break;
        }

        case dispatcher::tags::TagValueType::Float:
        {
            if (const auto numeric = runtime_value_to_double(value); numeric.has_value())
            {
                result.value = static_cast<float>(numeric.value());
                result.success = true;
                result.converted = true;
                return result;
            }

            break;
        }

        case dispatcher::tags::TagValueType::Double:
        {
            if (const auto numeric = runtime_value_to_double(value); numeric.has_value())
            {
                result.value = numeric.value();
                result.success = true;
                result.converted = true;
                return result;
            }

            break;
        }

        case dispatcher::tags::TagValueType::String:
        case dispatcher::tags::TagValueType::Enum:
        case dispatcher::tags::TagValueType::Json:
        {
            result.value = runtime_value_to_string(value);
            result.success = true;
            result.converted = true;
            return result;
        }
        }

        result.success = false;
        result.value = value;
        result.message = "Runtime value conversion failed.";

        return result;
    }
}