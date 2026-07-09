#pragma once

#include "scada_protocols/protocol_results.h"
#include "scada_runtime/tag_value_store.h"
#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"
#include "scada_tags/tag_quality.h"
#include "scada_tags/tag_value_type.h"

#include <cstdint>
#include <optional>
#include <string>

namespace dispatcher::runtime
{
    struct RuntimeValueApplyResult
    {
        dispatcher::tags::TagId tag_id;

        bool success = false;
        bool inserted = false;
        bool updated = false;

        std::uint64_t previous_change_counter = 0;
        std::uint64_t new_change_counter = 0;

        dispatcher::tags::TagQuality quality = dispatcher::tags::TagQuality::NotInitialized;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool was_inserted() const noexcept;
        [[nodiscard]] bool was_updated() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    class RuntimeValueApplier
    {
    public:
        explicit RuntimeValueApplier(
            TagValueStore& store
        ) noexcept;

        [[nodiscard]] RuntimeValueApplyResult apply_protocol_read_result(
            const dispatcher::protocols::ProtocolReadResult& read_result
        );

    private:
        TagValueStore& store_;

        [[nodiscard]] static dispatcher::tags::TagCurrentValue make_current_value(
            const dispatcher::protocols::ProtocolReadResult& read_result,
            const std::optional<dispatcher::tags::TagCurrentValue>& previous_value
        );

        [[nodiscard]] static dispatcher::tags::TagValueType infer_value_type(
            const dispatcher::tags::TagValuePayload& value,
            dispatcher::tags::TagValueType fallback
        ) noexcept;
    };
}