#pragma once

#include "scada_runtime/runtime_value_change.h"
#include "scada_tags/tag_current_value.h"
#include "scada_tags/tag_ids.h"
#include "scada_tags/tag_quality.h"

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace dispatcher::runtime
{
    enum class RuntimeEventType
    {
        ValueInitialized,
        ValueChanged,
        QualityChanged,
        ValueAndQualityChanged,
        SourceChanged
    };

    struct RuntimeValueEvent
    {
        dispatcher::tags::TagId tag_id;

        RuntimeEventType type = RuntimeEventType::ValueChanged;
        RuntimeValueChangeKind change_kind = RuntimeValueChangeKind::None;

        dispatcher::tags::TagQuality previous_quality = dispatcher::tags::TagQuality::NotInitialized;
        dispatcher::tags::TagQuality current_quality = dispatcher::tags::TagQuality::NotInitialized;

        dispatcher::tags::TagTimestamp timestamp{};

        std::uint64_t change_counter = 0;

        std::string message;

        [[nodiscard]] bool has_message() const noexcept;
    };

    [[nodiscard]] std::string_view to_string(
        RuntimeEventType type
    ) noexcept;

    [[nodiscard]] RuntimeEventType runtime_event_type_from_change_kind(
        RuntimeValueChangeKind kind
    ) noexcept;

    [[nodiscard]] std::optional<RuntimeValueEvent> make_runtime_value_event(
        const std::optional<dispatcher::tags::TagCurrentValue>& previous_value,
        const dispatcher::tags::TagCurrentValue& current_value,
        const RuntimeValueChangeResult& change_result
    );
}
