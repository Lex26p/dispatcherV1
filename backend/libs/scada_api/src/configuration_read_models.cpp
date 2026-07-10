#include "scada_api/configuration_read_models.h"

#include "scada_devices/device_connection.h"
#include "scada_devices/device_protocol.h"
#include "scada_devices/device_state.h"

#include "scada_objects/object_type.h"

#include "scada_tags/tag_archive_policy.h"
#include "scada_tags/tag_type.h"
#include "scada_tags/tag_value_type.h"

#include <algorithm>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace dispatcher::api {
namespace {

template <typename TId>
[[nodiscard]] std::optional<std::string> to_optional_id(
    const std::optional<TId>& value
)
{
    if (
        !value.has_value()
        || value->empty()
    ) {
        return std::nullopt;
    }

    return value->str();
}

[[nodiscard]] bool object_pointer_less(
    const dispatcher::objects::ObjectNode* left,
    const dispatcher::objects::ObjectNode* right
)
{
    if (left->code != right->code) {
        return left->code < right->code;
    }

    return left->id.str() < right->id.str();
}

[[nodiscard]] ObjectTreeNodeReadModel to_tree_read_model(
    const dispatcher::objects::ObjectNode& object
)
{
    return ObjectTreeNodeReadModel{
        .id = object.id.str(),
        .parent_id = to_optional_id(
            object.parent_id
        ),
        .type = std::string{
            dispatcher::objects::to_string(
                object.type
            )
        },
        .code = object.code,
        .name = object.name,
        .description = object.description,
        .children = {}
    };
}

using ObjectChildrenIndex = std::unordered_map<
    std::string,
    std::vector<const dispatcher::objects::ObjectNode*>
>;

[[nodiscard]] ObjectTreeNodeReadModel build_tree_node(
    const dispatcher::objects::ObjectNode& object,
    const ObjectChildrenIndex& children_index,
    std::unordered_set<std::string>& active_path
)
{
    auto result = to_tree_read_model(object);

    const auto object_id =
        object.id.str();

    if (!active_path.insert(object_id).second) {
        return result;
    }

    const auto children_it =
        children_index.find(object_id);

    if (children_it != children_index.end()) {
        result.children.reserve(
            children_it->second.size()
        );

        for (const auto* child : children_it->second) {
            result.children.push_back(
                build_tree_node(
                    *child,
                    children_index,
                    active_path
                )
            );
        }
    }

    active_path.erase(object_id);

    return result;
}

} // namespace

bool ObjectTreeNodeReadModel::has_children() const noexcept
{
    return !children.empty();
}

bool ObjectTreeReadResult::empty() const noexcept
{
    return roots.empty();
}

ObjectReadModel to_read_model(
    const dispatcher::objects::ObjectNode& object
)
{
    return ObjectReadModel{
        .id = object.id.str(),
        .parent_id = to_optional_id(
            object.parent_id
        ),
        .type = std::string{
            dispatcher::objects::to_string(
                object.type
            )
        },
        .code = object.code,
        .name = object.name,
        .description = object.description,
        .is_root = object.is_root()
    };
}

DeviceReadModel to_read_model(
    const dispatcher::devices::Device& device
)
{
    return DeviceReadModel{
        .id = device.id.str(),
        .object_id = to_optional_id(
            device.object_id
        ),
        .code = device.code,
        .name = device.name,
        .description = device.description,
        .protocol = std::string{
            dispatcher::devices::to_string(
                device.protocol
            )
        },
        .state = std::string{
            dispatcher::devices::to_string(
                device.state
            )
        },
        .runtime_enabled =
            device.is_enabled_for_runtime(),
        .connection = DeviceConnectionReadModel{
            .host = device.connection.host,
            .port = device.connection.port,
            .ip_mode = std::string{
                dispatcher::devices::to_string(
                    device.connection.ip_mode
                )
            },
            .network_interface =
                device.connection.network_interface
        }
    };
}

TagReadModel to_read_model(
    const dispatcher::tags::Tag& tag
)
{
    return TagReadModel{
        .id = tag.id.str(),
        .object_id = to_optional_id(
            tag.object_id
        ),
        .device_id = to_optional_id(
            tag.device_id
        ),
        .type = std::string{
            dispatcher::tags::to_string(
                tag.type
            )
        },
        .value_type = std::string{
            dispatcher::tags::to_string(
                tag.value_type
            )
        },
        .archive_policy = std::string{
            dispatcher::tags::to_string(
                tag.archive_policy
            )
        },
        .code = tag.code,
        .name = tag.name,
        .description = tag.description,
        .engineering_unit =
            tag.engineering_unit,
        .scale = tag.scale,
        .offset = tag.offset,
        .address = tag.address.address,
        .enabled = tag.is_enabled
    };
}

ObjectTreeReadResult build_object_tree_read_result(
    const std::vector<dispatcher::objects::ObjectNode>& objects
)
{
    ObjectTreeReadResult result;

    result.count =
        static_cast<std::uint64_t>(
            objects.size()
        );

    std::vector<
        const dispatcher::objects::ObjectNode*
    > ordered_objects;

    ordered_objects.reserve(
        objects.size()
    );

    for (const auto& object : objects) {
        ordered_objects.push_back(
            &object
        );
    }

    std::ranges::sort(
        ordered_objects,
        object_pointer_less
    );

    ObjectChildrenIndex children_index;

    std::vector<
        const dispatcher::objects::ObjectNode*
    > roots;

    for (const auto* object : ordered_objects) {
        if (
            object->parent_id.has_value()
            && !object->parent_id->empty()
        ) {
            children_index[
                object->parent_id->str()
            ].push_back(object);
        } else {
            roots.push_back(object);
        }
    }

    result.roots.reserve(
        roots.size()
    );

    std::unordered_set<std::string>
        active_path;

    for (const auto* root : roots) {
        result.roots.push_back(
            build_tree_node(
                *root,
                children_index,
                active_path
            )
        );
    }

    return result;
}

} // namespace dispatcher::api
