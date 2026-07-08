#include "scada_objects/object_tree.h"

#include "scada_objects/object_path.h"

#include <algorithm>
#include <ranges>
#include <utility>

namespace dispatcher::objects
{
    namespace
    {
        [[nodiscard]] bool contains_id(
            const std::vector<dispatcher::common::Uuid>& ids,
            const dispatcher::common::Uuid& id
        )
        {
            return std::ranges::any_of(
                ids,
                [&id](const dispatcher::common::Uuid& existing_id)
                {
                    return existing_id == id;
                }
            );
        }

        [[nodiscard]] bool has_duplicate_id(
            const std::vector<ObjectNode>& objects,
            const dispatcher::common::Uuid& id
        )
        {
            if (id.empty())
            {
                return false;
            }

            std::size_t count = 0;

            for (const auto& object : objects)
            {
                if (object.id == id)
                {
                    ++count;
                }

                if (count > 1)
                {
                    return true;
                }
            }

            return false;
        }

        [[nodiscard]] bool parent_exists(
            const std::vector<ObjectNode>& objects,
            const dispatcher::common::Uuid& parent_id
        )
        {
            return std::ranges::any_of(
                objects,
                [&parent_id](const ObjectNode& object)
                {
                    return object.id == parent_id;
                }
            );
        }

        [[nodiscard]] bool has_cycle(
            const std::vector<ObjectNode>& objects,
            const ObjectNode& start
        )
        {
            if (start.id.empty())
            {
                return false;
            }

            std::vector<dispatcher::common::Uuid> visited;
            visited.push_back(start.id);

            auto current = start;

            while (current.parent_id.has_value())
            {
                const auto parent_id = current.parent_id.value();

                if (parent_id.empty())
                {
                    return false;
                }

                if (contains_id(visited, parent_id))
                {
                    return true;
                }

                visited.push_back(parent_id);

                const auto parent = find_object_by_id(objects, parent_id);
                if (!parent.has_value())
                {
                    return false;
                }

                current = parent.value();
            }

            return false;
        }
    }

    bool ObjectTreeValidationResult::is_valid() const noexcept
    {
        return issues.empty();
    }

    ObjectTree::ObjectTree(std::vector<ObjectNode> objects)
        : objects_(std::move(objects))
    {
    }

    ObjectTree ObjectTree::from_objects(std::vector<ObjectNode> objects)
    {
        return ObjectTree{ std::move(objects) };
    }

    const std::vector<ObjectNode>& ObjectTree::objects() const noexcept
    {
        return objects_;
    }

    std::optional<ObjectNode> ObjectTree::find_by_id(
        const dispatcher::common::Uuid& id
    ) const
    {
        return find_object_by_id(objects_, id);
    }

    std::vector<ObjectNode> ObjectTree::roots() const
    {
        std::vector<ObjectNode> result;

        for (const auto& object : objects_)
        {
            if (object.is_root())
            {
                result.push_back(object);
            }
        }

        return result;
    }

    std::vector<ObjectNode> ObjectTree::children_of(
        const dispatcher::common::Uuid& parent_id
    ) const
    {
        std::vector<ObjectNode> result;

        for (const auto& object : objects_)
        {
            if (object.parent_id.has_value() && object.parent_id.value() == parent_id)
            {
                result.push_back(object);
            }
        }

        return result;
    }

    std::string ObjectTree::path_of(
        const dispatcher::common::Uuid& leaf_id
    ) const
    {
        return build_object_path(objects_, leaf_id);
    }

    ObjectTreeValidationResult ObjectTree::validate() const
    {
        ObjectTreeValidationResult result;

        for (const auto& object : objects_)
        {
            if (!object.has_valid_identity())
            {
                result.issues.push_back(
                    ObjectTreeValidationIssue{
                        .code = ObjectTreeValidationCode::EmptyIdentity,
                        .object_id = object.id,
                        .message = "Object identity is incomplete. Required fields: id, code, name."
                    }
                );
            }

            if (has_duplicate_id(objects_, object.id))
            {
                result.issues.push_back(
                    ObjectTreeValidationIssue{
                        .code = ObjectTreeValidationCode::DuplicateId,
                        .object_id = object.id,
                        .message = "Object id is duplicated."
                    }
                );
            }

            if (object.parent_id.has_value())
            {
                const auto parent_id = object.parent_id.value();

                if (parent_id == object.id)
                {
                    result.issues.push_back(
                        ObjectTreeValidationIssue{
                            .code = ObjectTreeValidationCode::SelfParent,
                            .object_id = object.id,
                            .message = "Object cannot be parent of itself."
                        }
                    );
                }

                if (!parent_id.empty() && !parent_exists(objects_, parent_id))
                {
                    result.issues.push_back(
                        ObjectTreeValidationIssue{
                            .code = ObjectTreeValidationCode::MissingParent,
                            .object_id = object.id,
                            .message = "Object parent does not exist in tree."
                        }
                    );
                }
            }

            if (has_cycle(objects_, object))
            {
                result.issues.push_back(
                    ObjectTreeValidationIssue{
                        .code = ObjectTreeValidationCode::CycleDetected,
                        .object_id = object.id,
                        .message = "Object tree contains cycle."
                    }
                );
            }
        }

        return result;
    }

    std::string_view to_string(ObjectTreeValidationCode code) noexcept
    {
        switch (code)
        {
        case ObjectTreeValidationCode::EmptyIdentity:
            return "EmptyIdentity";
        case ObjectTreeValidationCode::DuplicateId:
            return "DuplicateId";
        case ObjectTreeValidationCode::MissingParent:
            return "MissingParent";
        case ObjectTreeValidationCode::SelfParent:
            return "SelfParent";
        case ObjectTreeValidationCode::CycleDetected:
            return "CycleDetected";
        }

        return "Unknown";
    }
}