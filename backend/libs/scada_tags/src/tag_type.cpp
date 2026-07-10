#include "scada_tags/tag_type.h"

namespace dispatcher::tags
{
    std::string_view to_string(TagType type) noexcept
    {
        switch (type)
        {
        case TagType::DeviceTag:
            return "DeviceTag";
        case TagType::ManualTag:
            return "ManualTag";
        case TagType::CalculatedTag:
            return "CalculatedTag";
        case TagType::SystemTag:
            return "SystemTag";
        case TagType::VirtualTag:
            return "VirtualTag";
        case TagType::CommandTag:
            return "CommandTag";
        case TagType::SimulationTag:
            return "SimulationTag";
        case TagType::ExternalTag:
            return "ExternalTag";
        }

        return "Unknown";
    }
}
