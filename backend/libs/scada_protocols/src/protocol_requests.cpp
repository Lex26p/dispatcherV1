#include "scada_protocols/protocol_requests.h"

namespace dispatcher::protocols
{
    bool ProtocolBatchReadRequest::empty() const noexcept
    {
        return items.empty();
    }

    std::size_t ProtocolBatchReadRequest::size() const noexcept
    {
        return items.size();
    }
}
