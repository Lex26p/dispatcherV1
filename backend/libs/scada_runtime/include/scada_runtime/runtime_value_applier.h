#pragma once

#include "scada_protocols/protocol_results.h"
#include "scada_runtime/runtime_event.h"
#include "scada_runtime/runtime_value_change.h"
#include "scada_runtime/tag_value_store.h"
#include "scada_tags/tag.h"
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
        bool changed = false;
        bool good_quality = false;
        bool last_good_updated = false;
        bool value_converted = false;
        bool engineering_transform_applied = false;

        RuntimeValueChangeKind change_kind = RuntimeValueChangeKind::None;
        std::optional<RuntimeValueEvent> event;

        std::uint64_t previous_change_counter = 0;
        std::uint64_t new_change_counter = 0;

        dispatcher::tags::TagQuality quality = dispatcher::tags::TagQuality::NotInitialized;

        std::string message;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool was_inserted() const noexcept;
        [[nodiscard]] bool was_updated() const noexcept;
        [[nodiscard]] bool has_changed() const noexcept;
        [[nodiscard]] bool has_good_quality() const noexcept;
        [[nodiscard]] bool was_last_good_updated() const noexcept;
        [[nodiscard]] bool was_value_converted() const noexcept;
        [[nodiscard]] bool was_engineering_transform_applied() const noexcept;
        [[nodiscard]] bool has_event() const noexcept;
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

        [[nodiscard]] RuntimeValueApplyResult apply_protocol_read_result(
            const dispatcher::protocols::ProtocolReadResult& read_result,
            const dispatcher::tags::Tag& tag
        );

    private:
        TagValueStore& store_;

        [[nodiscard]] RuntimeValueApplyResult apply_protocol_read_result_internal(
            const dispatcher::protocols::ProtocolReadResult& read_result,
            const dispatcher::tags::Tag* tag
        );

        [[nodiscard]] static dispatcher::tags::TagCurrentValue make_current_value(
            const dispatcher::protocols::ProtocolReadResult& read_result,
            const std::optional<dispatcher::tags::TagCurrentValue>& previous_value,
            const dispatcher::tags::Tag* tag,
            bool& value_converted,
            bool& engineering_transform_applied
        );

        [[nodiscard]] static bool did_update_last_good_value(
            const std::optional<dispatcher::tags::TagCurrentValue>& previous_value,
            const dispatcher::tags::TagCurrentValue& current_value
        ) noexcept;
    };
}
