#pragma once

#include <string_view>

namespace dispatcher::tags
{
    enum class TagValueType
    {
        Boolean,
        Int32,
        Int64,
        Float,
        Double,
        String,
        Enum,
        Json
    };

    [[nodiscard]] std::string_view to_string(TagValueType type) noexcept;
}