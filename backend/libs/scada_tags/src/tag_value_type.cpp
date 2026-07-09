#include "scada_tags/tag_value_type.h"

namespace dispatcher::tags
{
    std::string_view to_string(TagValueType type) noexcept
    {
        switch (type)
        {
        case TagValueType::Boolean:
            return "Boolean";
        case TagValueType::Int32:
            return "Int32";
        case TagValueType::Int64:
            return "Int64";
        case TagValueType::Float:
            return "Float";
        case TagValueType::Double:
            return "Double";
        case TagValueType::String:
            return "String";
        case TagValueType::Enum:
            return "Enum";
        case TagValueType::Json:
            return "Json";
        }

        return "Unknown";
    }
}