#pragma once

#include <string>
#include <string_view>

namespace dispatcher::common
{
    template <typename Tag>
    class EntityId
    {
    public:
        EntityId() = default;

        explicit EntityId(std::string value)
            : value_(std::move(value))
        {
        }

        [[nodiscard]] static EntityId from_string(std::string_view value)
        {
            return EntityId{ std::string{ value } };
        }

        [[nodiscard]] const std::string& str() const noexcept
        {
            return value_;
        }

        [[nodiscard]] bool empty() const noexcept
        {
            return value_.empty();
        }

        friend bool operator==(const EntityId& left, const EntityId& right) noexcept = default;

    private:
        std::string value_;
    };
}
