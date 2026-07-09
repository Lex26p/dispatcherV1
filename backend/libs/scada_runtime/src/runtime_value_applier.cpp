#include "scada_runtime/runtime_value_applier.h"

#include "scada_runtime/engineering_transform.h"
#include "scada_runtime/runtime_event.h"
#include "scada_runtime/runtime_quality.h"
#include "scada_runtime/runtime_value_change.h"
#include "scada_runtime/runtime_value_conversion.h"

#include <chrono>

namespace dispatcher::runtime
{
    namespace
    {
        [[nodiscard]] dispatcher::tags::TagTimestamp now()
        {
            return std::chrono::system_clock::now();
        }

        [[nodiscard]] bool is_empty_timestamp(
            dispatcher::tags::TagTimestamp timestamp
        ) noexcept
        {
            return timestamp == dispatcher::tags::TagTimestamp{};
        }
    }

    bool RuntimeValueApplyResult::is_success() const noexcept
    {
        return success;
    }

    bool RuntimeValueApplyResult::was_inserted() const noexcept
    {
        return inserted;
    }

    bool RuntimeValueApplyResult::was_updated() const noexcept
    {
        return updated;
    }

    bool RuntimeValueApplyResult::has_changed() const noexcept
    {
        return changed;
    }

    bool RuntimeValueApplyResult::has_good_quality() const noexcept
    {
        return good_quality;
    }

    bool RuntimeValueApplyResult::was_last_good_updated() const noexcept
    {
        return last_good_updated;
    }

    bool RuntimeValueApplyResult::was_value_converted() const noexcept
    {
        return value_converted;
    }

    bool RuntimeValueApplyResult::was_engineering_transform_applied() const noexcept
    {
        return engineering_transform_applied;
    }

    bool RuntimeValueApplyResult::has_event() const noexcept
    {
        return event.has_value();
    }

    bool RuntimeValueApplyResult::has_message() const noexcept
    {
        return !message.empty();
    }

    RuntimeValueApplier::RuntimeValueApplier(
        TagValueStore& store
    ) noexcept
        : store_(store)
    {
    }

    RuntimeValueApplyResult RuntimeValueApplier::apply_protocol_read_result(
        const dispatcher::protocols::ProtocolReadResult& read_result
    )
    {
        return apply_protocol_read_result_internal(
            read_result,
            nullptr
        );
    }

    RuntimeValueApplyResult RuntimeValueApplier::apply_protocol_read_result(
        const dispatcher::protocols::ProtocolReadResult& read_result,
        const dispatcher::tags::Tag& tag
    )
    {
        return apply_protocol_read_result_internal(
            read_result,
            &tag
        );
    }

    RuntimeValueApplyResult RuntimeValueApplier::apply_protocol_read_result_internal(
        const dispatcher::protocols::ProtocolReadResult& read_result,
        const dispatcher::tags::Tag* tag
    )
    {
        RuntimeValueApplyResult result;
        result.tag_id = read_result.tag_id;
        result.quality = read_result.quality;
        result.good_quality = is_good_runtime_quality(read_result.quality);

        if (read_result.tag_id.empty())
        {
            result.success = false;
            result.message = "Protocol read result has empty tag id.";
            return result;
        }

        if (tag != nullptr && tag->id != read_result.tag_id)
        {
            result.success = false;
            result.message = "Protocol read result tag id does not match tag configuration.";
            return result;
        }

        const auto previous_value = store_.find_by_tag_id(read_result.tag_id);

        bool value_converted = false;
        bool engineering_transform_applied = false;

        auto current_value = make_current_value(
            read_result,
            previous_value,
            tag,
            value_converted,
            engineering_transform_applied
        );

        const auto change_result = detect_runtime_value_change(
            previous_value,
            current_value
        );

        if (change_result.has_changed())
        {
            current_value.mark_changed();
        }

        const auto runtime_event = make_runtime_value_event(
            previous_value,
            current_value,
            change_result
        );

        if (!store_.upsert(current_value))
        {
            result.success = false;
            result.message = "Failed to upsert runtime tag value.";
            return result;
        }

        result.success = true;
        result.inserted = !previous_value.has_value();
        result.updated = previous_value.has_value();
        result.changed = change_result.has_changed();
        result.change_kind = change_result.kind;
        result.event = runtime_event;
        result.previous_change_counter = previous_value.has_value()
            ? previous_value->change_counter
            : 0;
        result.new_change_counter = current_value.change_counter;
        result.last_good_updated = did_update_last_good_value(
            previous_value,
            current_value
        );
        result.value_converted = value_converted;
        result.engineering_transform_applied = engineering_transform_applied;

        if (change_result.has_changed())
        {
            result.message = change_result.message;
        }
        else if (read_result.has_message())
        {
            result.message = read_result.message;
        }
        else
        {
            result.message = "Protocol read result applied to runtime store without value change.";
        }

        return result;
    }

    dispatcher::tags::TagCurrentValue RuntimeValueApplier::make_current_value(
        const dispatcher::protocols::ProtocolReadResult& read_result,
        const std::optional<dispatcher::tags::TagCurrentValue>& previous_value,
        const dispatcher::tags::Tag* tag,
        bool& value_converted,
        bool& engineering_transform_applied
    )
    {
        dispatcher::tags::TagCurrentValue current;

        if (previous_value.has_value())
        {
            current = *previous_value;
        }

        const auto timestamp = is_empty_timestamp(read_result.server_timestamp)
            ? now()
            : read_result.server_timestamp;

        const auto source_timestamp = is_empty_timestamp(read_result.source_timestamp)
            ? timestamp
            : read_result.source_timestamp;

        const auto fallback_value_type = previous_value.has_value()
            ? previous_value->value_type
            : dispatcher::tags::TagValueType::Double;

        const auto target_value_type = tag != nullptr
            ? tag->value_type
            : infer_runtime_value_type(
                read_result.raw_value,
                fallback_value_type
            );

        auto raw_value = read_result.raw_value;

        if (tag != nullptr)
        {
            const auto conversion = convert_runtime_value(
                read_result.raw_value,
                target_value_type
            );

            if (conversion.is_success())
            {
                raw_value = conversion.value;
                value_converted = conversion.was_converted();
            }
        }

        auto engineering_value = raw_value;

        if (tag != nullptr)
        {
            const auto transform = apply_engineering_transform(
                raw_value,
                RuntimeEngineeringTransformOptions{
                    .enabled = true,
                    .scale = tag->scale,
                    .offset = tag->offset
                }
            );

            if (transform.is_success())
            {
                engineering_value = transform.engineering_value;
                engineering_transform_applied = transform.was_transformed();
            }
        }

        current.tag_id = read_result.tag_id;
        current.value_type = target_value_type;
        current.quality = read_result.quality;
        current.source = dispatcher::tags::TagValueSource::Device;

        current.raw_value = raw_value;
        current.engineering_value = engineering_value;

        current.timestamp = timestamp;
        current.source_timestamp = source_timestamp;
        current.server_timestamp = timestamp;

        if (can_update_last_good_value(
            current.quality,
            current.engineering_value
        ))
        {
            current.last_good_value = current.engineering_value;
            current.last_good_timestamp = timestamp;
        }

        return current;
    }

    bool RuntimeValueApplier::did_update_last_good_value(
        const std::optional<dispatcher::tags::TagCurrentValue>& previous_value,
        const dispatcher::tags::TagCurrentValue& current_value
    ) noexcept
    {
        if (!current_value.has_last_good_value())
        {
            return false;
        }

        if (!previous_value.has_value())
        {
            return true;
        }

        if (!previous_value->has_last_good_value())
        {
            return true;
        }

        return current_value.last_good_timestamp != previous_value->last_good_timestamp;
    }
}