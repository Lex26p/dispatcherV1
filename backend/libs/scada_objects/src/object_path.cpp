#include "scada_objects/object_path.h"

#include <algorithm>
#include <ranges>
#include <sstream>

namespace dispatcher::objects
{
    std::optional<ObjectNode> find_object_by_id(
        const std::vector<ObjectNode>& objects,
        const ObjectId& id
    )
    {
        const auto iterator = std::ranges::find_if(
            objects,
            [&id](const ObjectNode& object)
            {
                return object.id == id;
            }
        );

        if (iterator == objects.end())
        {
            return std::nullopt;
        }

        return *iterator;
    }

    std::string build_object_path(
        const std::vector<ObjectNode>& objects,
        const ObjectId& leaf_id
    )
    {
        std::vector<std::string> names;

        auto current = find_object_by_id(objects, leaf_id);
        while (current.has_value())
        {
            names.push_back(current->name);

            if (!current->parent_id.has_value())
            {
                break;
            }

            current = find_object_by_id(objects, current->parent_id.value());
        }

        std::ranges::reverse(names);

        std::ostringstream stream;
        for (std::size_t index = 0; index < names.size(); ++index)
        {
            if (index > 0)
            {
                stream << " / ";
            }

            stream << names[index];
        }

        return stream.str();
    }
}
