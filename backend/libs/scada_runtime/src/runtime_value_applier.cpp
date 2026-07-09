#include "scada_runtime/runtime_value_applier.h"

#include "scada_runtime/runtime_quality.h"

#include <chrono>
#include <variant>

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

    bool RuntimeValueApplyResult::has_good_quality() const noexcept
    {
        return good_quality;
    }

    bool RuntimeValueApplyResult::was_last_good_updated() const noexcept
    {
        return last_good_updated;
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

        const auto previous_value = store_.find_by_tag_id(read_result.tag_id);

        const auto current_value = make_current_value(
            read_result,
            previous_value
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
        result.previous_change_counter = previous_value.has_value()
            ? previous_value->change_counter
            : 0;
        result.new_change_counter = current_value.change_counter;
        result.last_good_updated = did_update_last_good_value(
            previous_value,
            current_value
        );

        result.message = read_result.has_message()
            ? read_result.message
            : "Protocol read result applied to runtime store.";

        return result;
    }

    dispatcher::tags::TagCurrentValue RuntimeValueApplier::make_current_value(
        const dispatcher::protocols::ProtocolReadResult& read_result,
        const std::optional<dispatcher::tags::TagCurrentValue>& previous_value
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

        current.tag_id = read_result.tag_id;
        current.value_type = infer_value_type(
            read_result.raw_value,
            fallback_value_type
        );
        current.quality = read_result.quality;
        current.source = dispatcher::tags::TagValueSource::Device;

        current.raw_value = read_result.raw_value;
        current.engineering_value = read_result.raw_value;

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

        current.mark_changed();

        return current;
    }

    dispatcher::tags::TagValueType RuntimeValueApplier::infer_value_type(
        const dispatcher::tags::TagValuePayload& value,
        dispatcher::tags::TagValueType fallback
    ) noexcept
    {
        if (std::holds_alternative<bool>(value))
        {
            return dispatcher::tags::TagValueType::Boolean;
        }

        if (std::holds_alternative<std::int32_t>(value))
        {
            return dispatcher::tags::TagValueType::Int32;
        }

        if (std::holds_alternative<std::int64_t>(value))
        {
            return dispatcher::tags::TagValueType::Int64;
        }

        if (std::holds_alternative<float>(value))
        {
            return dispatcher::tags::TagValueType::Float;
        }

        if (std::holds_alternative<double>(value))
        {
            return dispatcher::tags::TagValueType::Double;
        }

        if (std::holds_alternative<std::string>(value))
        {
            return dispatcher::tags::TagValueType::String;
        }

        return fallback;
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