#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace dispatcher::core
{
    class ISettingsProvider
    {
    public:
        virtual ~ISettingsProvider() = default;

        [[nodiscard]] virtual std::optional<std::string> get_string(std::string_view key) const = 0;
        [[nodiscard]] virtual bool get_bool_or_default(std::string_view key, bool default_value) const = 0;
        [[nodiscard]] virtual int get_int_or_default(std::string_view key, int default_value) const = 0;
    };
}
