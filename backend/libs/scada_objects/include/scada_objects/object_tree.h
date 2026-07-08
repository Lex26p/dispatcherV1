#pragma once

#include "scada_common/id_types.h"
#include "scada_objects/object_node.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::objects
{
    enum class ObjectTreeValidationCode
    {
        EmptyIdentity,
        DuplicateId,
        MissingParent,
        SelfParent,
        CycleDetected,
        InvalidRootType,
        InvalidParentChildRelation
    };

    struct ObjectTreeValidationIssue
    {
        ObjectTreeValidationCode code = ObjectTreeValidationCode::EmptyIdentity;
        dispatcher::common::Uuid object_id;
        std::string message;
    };

    struct ObjectTreeValidationResult
    {
        std::vector<ObjectTreeValidationIssue> issues;

        [[nodiscard]] bool is_valid() const noexcept;
    };

    class ObjectTree
    {
    public:
        ObjectTree() = default;
        explicit ObjectTree(std::vector<ObjectNode> objects);

        [[nodiscard]] static ObjectTree from_objects(std::vector<ObjectNode> objects);

        [[nodiscard]] const std::vector<ObjectNode>& objects() const noexcept;

        [[nodiscard]] std::optional<ObjectNode> find_by_id(
            const dispatcher::common::Uuid& id
        ) const;

        [[nodiscard]] std::vector<ObjectNode> roots() const;

        [[nodiscard]] std::vector<ObjectNode> children_of(
            const dispatcher::common::Uuid& parent_id
        ) const;

        [[nodiscard]] std::string path_of(
            const dispatcher::common::Uuid& leaf_id
        ) const;

        [[nodiscard]] ObjectTreeValidationResult validate() const;

    private:
        std::vector<ObjectNode> objects_;
    };

    [[nodiscard]] std::string_view to_string(ObjectTreeValidationCode code) noexcept;
}