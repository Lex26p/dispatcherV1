#pragma once

#include <string>
#include <string_view>

namespace dispatcher::common
{
    class Uuid
    {
    public:
        Uuid() = default;
        explicit Uuid(std::string value);

        static Uuid from_string(std::string_view value);

        [[nodiscard]] const std::string& str() const noexcept;
        [[nodiscard]] bool empty() const noexcept;

        friend bool operator==(const Uuid& left, const Uuid& right) noexcept = default;

    private:
        std::string value_;
    };

    class CorrelationId
    {
    public:
        CorrelationId() = default;
        explicit CorrelationId(std::string value);

        static CorrelationId from_string(std::string_view value);
        static CorrelationId startup();

        [[nodiscard]] const std::string& str() const noexcept;
        [[nodiscard]] bool empty() const noexcept;

        friend bool operator==(const CorrelationId& left, const CorrelationId& right) noexcept = default;

    private:
        std::string value_;
    };
}
