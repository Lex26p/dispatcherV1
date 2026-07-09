#include "scada_runtime/runtime_event.h"

namespace dispatcher::runtime
{
    bool RuntimeValueEvent::has_message() const noexcept
    {
        return !message.empty();
    }

    std::string_view to_string(
        RuntimeEventType type
    ) noexcept
    {
        switch (type)
        {
        case RuntimeEventType::ValueInitialized:
            return "ValueInitialized";
        case RuntimeEventType::ValueChanged:
            return "ValueChanged";
        case RuntimeEventType::QualityChanged:
            return "QualityChanged";
        case RuntimeEventType::ValueAndQualityChanged:
            return "ValueAndQualityChanged";
        case RuntimeEventType::SourceChanged:
            return "SourceChanged";
        }

        return "Unknown";
    }

    RuntimeEventType runtime_event_type_from_change_kind(
        RuntimeValueChangeKind kind
    ) noexcept
    {
        switch (kind)
        {
        case RuntimeValueChangeKind::Initialized:
            return RuntimeEventType::ValueInitialized;
        case RuntimeValueChangeKind::ValueChanged:
            return RuntimeEventType::ValueChanged;
        case RuntimeValueChangeKind::QualityChanged:
            return RuntimeEventType::QualityChanged;
        case RuntimeValueChangeKind::ValueAndQualityChanged:
            return RuntimeEventType::ValueAndQualityChanged;
        case RuntimeValueChangeKind::SourceChanged:
            return RuntimeEventType::SourceChanged;
        case RuntimeValueChangeKind::None:
            return RuntimeEventType::ValueChanged;
        }

        return RuntimeEventType::ValueChanged;
    }

    std::optional<RuntimeValueEvent> make_runtime_value_event(
        const std::optional<dispatcher::tags::TagCurrentValue>& previous_value,
        const dispatcher::tags::TagCurrentValue& current_value,
        const RuntimeValueChangeResult& change_result
    )
    {
        if (!change_result.has_changed())
        {
            return std::nullopt;
        }

        RuntimeValueEvent event;

        event.tag_id = current_value.tag_id;
        event.type = runtime_event_type_from_change_kind(change_result.kind);
        event.change_kind = change_result.kind;

        event.previous_quality = previous_value.has_value()
            ? previous_value->quality
            : dispatcher::tags::TagQuality::NotInitialized;

        event.current_quality = current_value.quality;
        event.timestamp = current_value.server_timestamp;
        event.change_counter = current_value.change_counter;
        event.message = change_result.message;

        return event;
    }
}