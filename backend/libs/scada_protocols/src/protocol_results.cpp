#include "scada_protocols/protocol_results.h"

#include <variant>

namespace dispatcher::protocols
{
    bool ProtocolConnectionCheckResult::is_success() const noexcept
    {
        return success;
    }

    bool ProtocolConnectionCheckResult::has_message() const noexcept
    {
        return !message.empty();
    }

    bool ProtocolAddressTestResult::is_success() const noexcept
    {
        return success;
    }

    bool ProtocolAddressTestResult::has_message() const noexcept
    {
        return !message.empty();
    }

    bool ProtocolReadResult::is_success() const noexcept
    {
        return success;
    }

    bool ProtocolReadResult::has_value() const noexcept
    {
        return !std::holds_alternative<std::monostate>(raw_value);
    }

    bool ProtocolReadResult::has_message() const noexcept
    {
        return !message.empty();
    }

    bool ProtocolBatchReadResult::is_success() const noexcept
    {
        return success;
    }

    bool ProtocolBatchReadResult::empty() const noexcept
    {
        return items.empty();
    }

    std::size_t ProtocolBatchReadResult::size() const noexcept
    {
        return items.size();
    }

    bool ProtocolBatchReadResult::has_message() const noexcept
    {
        return !message.empty();
    }

    bool ProtocolWriteResult::is_success() const noexcept
    {
        return success;
    }

    bool ProtocolWriteResult::has_message() const noexcept
    {
        return !message.empty();
    }
}