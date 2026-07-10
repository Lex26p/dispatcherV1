#include "scada_common/id_types.h"

namespace dispatcher::common
{
    Uuid::Uuid(std::string value)
        : value_(std::move(value))
    {
    }

    Uuid Uuid::from_string(std::string_view value)
    {
        return Uuid{ std::string{value} };
    }

    const std::string& Uuid::str() const noexcept
    {
        return value_;
    }

    bool Uuid::empty() const noexcept
    {
        return value_.empty();
    }

    CorrelationId::CorrelationId(std::string value)
        : value_(std::move(value))
    {
    }

    CorrelationId CorrelationId::from_string(std::string_view value)
    {
        return CorrelationId{ std::string{value} };
    }

    CorrelationId CorrelationId::startup()
    {
        return CorrelationId{ "startup" };
    }

    const std::string& CorrelationId::str() const noexcept
    {
        return value_;
    }

    bool CorrelationId::empty() const noexcept
    {
        return value_.empty();
    }
}
