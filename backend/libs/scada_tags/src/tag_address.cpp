#include "scada_tags/tag_address.h"

namespace dispatcher::tags
{
    bool TagAddress::has_address() const noexcept
    {
        return !address.empty();
    }

    bool TagAddress::has_protocol_payload() const noexcept
    {
        return !protocol_address_json.empty();
    }

    bool TagAddress::uses_bit_extraction() const noexcept
    {
        return bit_index.has_value() ||
            byte_offset.has_value() ||
            mask.has_value() ||
            shift.has_value() ||
            bit_length.has_value();
    }
}
