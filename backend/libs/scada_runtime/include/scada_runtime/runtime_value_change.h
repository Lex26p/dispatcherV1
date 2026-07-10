#pragma once

#include "scada_tags/tag_current_value.h"

#include <optional>
#include <string>
#include <string_view>

namespace dispatcher::runtime
{
    enum class RuntimeValueChangeKind
    {
        None,
        Initialized,
        ValueChanged,
        QualityChanged,
        ValueAndQualityChanged,
        SourceChanged
    };

    struct RuntimeValueChangeResult
    {
        RuntimeValueChangeKind kind = RuntimeValueChangeKind::None;

        bool changed = false;
        bool value_changed = false;
        bool quality_changed = false;
        bool source_changed = false;

        std::string message;

        [[nodiscard]] bool has_changed() const noexcept;
        [[nodiscard]] bool has_value_changed() const noexcept;
        [[nodiscard]] bool has_quality_changed() const noexcept;
        [[nodiscard]] bool has_source_changed() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
    };

    [[nodiscard]] std::string_view to_string(
        RuntimeValueChangeKind kind
    ) noexcept;

    [[nodiscard]] RuntimeValueChangeResult detect_runtime_value_change(
        const std::optional<dispatcher::tags::TagCurrentValue>& previous_value,
        const dispatcher::tags::TagCurrentValue& current_value
    );
}
