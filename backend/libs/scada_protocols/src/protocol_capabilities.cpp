#include "scada_protocols/protocol_capabilities.h"

namespace dispatcher::protocols
{
    bool ProtocolCapabilities::can_read() const noexcept
    {
        return supports_read;
    }

    bool ProtocolCapabilities::can_write() const noexcept
    {
        return supports_write;
    }

    bool ProtocolCapabilities::can_batch_read() const noexcept
    {
        return supports_batch_read && max_batch_size > 1;
    }

    bool ProtocolCapabilities::can_check_connection() const noexcept
    {
        return supports_connection_check;
    }

    bool ProtocolCapabilities::can_test_address() const noexcept
    {
        return supports_address_test;
    }

    std::string_view to_string(
        const ProtocolCapabilities&
    ) noexcept
    {
        return "ProtocolCapabilities";
    }
}