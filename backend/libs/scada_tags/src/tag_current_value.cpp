#include "scada_tags/tag_current_value.h"

namespace dispatcher::tags
{
    bool TagCurrentValue::has_value() const noexcept
    {
        return !std::holds_alternative<std::monostate>(engineering_value);
    }

    bool TagCurrentValue::has_good_quality() const noexcept
    {
        return quality == TagQuality::Good ||
            quality == TagQuality::Manual ||
            quality == TagQuality::Simulation;
    }

    bool TagCurrentValue::has_last_good_value() const noexcept
    {
        return last_good_value.has_value();
    }

    bool TagCurrentValue::is_initialized() const noexcept
    {
        return quality != TagQuality::NotInitialized;
    }

    bool TagCurrentValue::is_from_runtime_source() const noexcept
    {
        return source == TagValueSource::Device ||
            source == TagValueSource::Calculation ||
            source == TagValueSource::System ||
            source == TagValueSource::Simulation ||
            source == TagValueSource::External;
    }

    void TagCurrentValue::mark_changed() noexcept
    {
        ++change_counter;
    }

    std::string_view to_string(
        TagValueSource source
    ) noexcept
    {
        switch (source)
        {
        case TagValueSource::Unknown:
            return "Unknown";
        case TagValueSource::Device:
            return "Device";
        case TagValueSource::Manual:
            return "Manual";
        case TagValueSource::Calculation:
            return "Calculation";
        case TagValueSource::System:
            return "System";
        case TagValueSource::Simulation:
            return "Simulation";
        case TagValueSource::External:
            return "External";
        }

        return "Unknown";
    }
}
