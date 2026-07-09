#include "scada_runtime/runtime_value_change.h"

namespace dispatcher::runtime
{
    bool RuntimeValueChangeResult::has_changed() const noexcept
    {
        return changed;
    }

    bool RuntimeValueChangeResult::has_value_changed() const noexcept
    {
        return value_changed;
    }

    bool RuntimeValueChangeResult::has_quality_changed() const noexcept
    {
        return quality_changed;
    }

    bool RuntimeValueChangeResult::has_source_changed() const noexcept
    {
        return source_changed;
    }

    bool RuntimeValueChangeResult::has_message() const noexcept
    {
        return !message.empty();
    }

    std::string_view to_string(
        RuntimeValueChangeKind kind
    ) noexcept
    {
        switch (kind)
        {
        case RuntimeValueChangeKind::None:
            return "None";
        case RuntimeValueChangeKind::Initialized:
            return "Initialized";
        case RuntimeValueChangeKind::ValueChanged:
            return "ValueChanged";
        case RuntimeValueChangeKind::QualityChanged:
            return "QualityChanged";
        case RuntimeValueChangeKind::ValueAndQualityChanged:
            return "ValueAndQualityChanged";
        case RuntimeValueChangeKind::SourceChanged:
            return "SourceChanged";
        }

        return "Unknown";
    }

    RuntimeValueChangeResult detect_runtime_value_change(
        const std::optional<dispatcher::tags::TagCurrentValue>& previous_value,
        const dispatcher::tags::TagCurrentValue& current_value
    )
    {
        RuntimeValueChangeResult result;

        if (!previous_value.has_value())
        {
            result.kind = RuntimeValueChangeKind::Initialized;
            result.changed = true;
            result.value_changed = true;
            result.quality_changed = true;
            result.source_changed = true;
            result.message = "Runtime value initialized.";
            return result;
        }

        result.value_changed = previous_value->engineering_value != current_value.engineering_value;
        result.quality_changed = previous_value->quality != current_value.quality;
        result.source_changed = previous_value->source != current_value.source;

        result.changed = result.value_changed ||
            result.quality_changed ||
            result.source_changed;

        if (!result.changed)
        {
            result.kind = RuntimeValueChangeKind::None;
            result.message = "Runtime value did not change.";
            return result;
        }

        if (result.value_changed && result.quality_changed)
        {
            result.kind = RuntimeValueChangeKind::ValueAndQualityChanged;
            result.message = "Runtime value and quality changed.";
            return result;
        }

        if (result.value_changed)
        {
            result.kind = RuntimeValueChangeKind::ValueChanged;
            result.message = "Runtime value changed.";
            return result;
        }

        if (result.quality_changed)
        {
            result.kind = RuntimeValueChangeKind::QualityChanged;
            result.message = "Runtime quality changed.";
            return result;
        }

        if (result.source_changed)
        {
            result.kind = RuntimeValueChangeKind::SourceChanged;
            result.message = "Runtime value source changed.";
            return result;
        }

        result.kind = RuntimeValueChangeKind::None;
        result.message = "Runtime value did not change.";

        return result;
    }
}