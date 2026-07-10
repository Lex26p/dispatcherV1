#include "scada_app/development_configuration_read_service.h"

#include "scada_devices/device_validation.h"
#include "scada_objects/object_tree.h"
#include "scada_tags/tag_validation.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

namespace dispatcher::app {
namespace {

using dispatcher::api::DeviceReadModel;
using dispatcher::api::DeviceReadQuery;
using dispatcher::api::ObjectReadModel;
using dispatcher::api::ObjectReadQuery;
using dispatcher::api::ObjectTreeNodeReadModel;
using dispatcher::api::ObjectTreeReadResult;
using dispatcher::api::ReadCollection;
using dispatcher::api::TagReadModel;
using dispatcher::api::TagReadQuery;

using dispatcher::devices::Device;
using dispatcher::devices::DeviceConnection;
using dispatcher::devices::DeviceId;
using dispatcher::devices::DeviceProtocol;
using dispatcher::devices::DeviceState;
using dispatcher::devices::IpMode;

using dispatcher::objects::ObjectId;
using dispatcher::objects::ObjectNode;
using dispatcher::objects::ObjectTree;
using dispatcher::objects::ObjectType;

using dispatcher::tags::Tag;
using dispatcher::tags::TagAddress;
using dispatcher::tags::TagArchivePolicy;
using dispatcher::tags::TagId;
using dispatcher::tags::TagType;
using dispatcher::tags::TagValueType;

[[nodiscard]] ObjectId make_object_id(
    const std::string_view value
)
{
    return ObjectId::from_string(value);
}

[[nodiscard]] DeviceId make_device_id(
    const std::string_view value
)
{
    return DeviceId::from_string(value);
}

[[nodiscard]] TagId make_tag_id(
    const std::string_view value
)
{
    return TagId::from_string(value);
}

[[nodiscard]] std::vector<ObjectNode>
make_development_objects()
{
    return {
        ObjectNode{
            .id = make_object_id(
                "site-main"
            ),
            .parent_id = std::nullopt,
            .type = ObjectType::Site,
            .code = "SITE_MAIN",
            .name = "Main site",
            .description =
                "Primary Dispatcher development site."
        },
        ObjectNode{
            .id = make_object_id(
                "building-admin"
            ),
            .parent_id = make_object_id(
                "site-main"
            ),
            .type = ObjectType::Building,
            .code = "BUILDING_ADMIN",
            .name = "Administration building",
            .description =
                "Administration and control building."
        },
        ObjectNode{
            .id = make_object_id(
                "room-control"
            ),
            .parent_id = make_object_id(
                "building-admin"
            ),
            .type = ObjectType::Room,
            .code = "CONTROL_ROOM",
            .name = "Control room",
            .description =
                "Main operator control room."
        },
        ObjectNode{
            .id = make_object_id(
                "line-water"
            ),
            .parent_id = std::nullopt,
            .type = ObjectType::Line,
            .code = "WATER_LINE",
            .name = "Water line",
            .description =
                "Development water process line."
        },
        ObjectNode{
            .id = make_object_id(
                "group-pumps"
            ),
            .parent_id = make_object_id(
                "line-water"
            ),
            .type =
                ObjectType::EquipmentGroup,
            .code = "PUMP_GROUP",
            .name = "Pump group",
            .description =
                "Water line pump equipment."
        }
    };
}

[[nodiscard]] std::vector<Device>
make_development_devices()
{
    return {
        Device{
            .id = make_device_id(
                "device-modbus-1"
            ),
            .object_id = make_object_id(
                "room-control"
            ),
            .code = "PLC_ROOM",
            .name = "Control room PLC",
            .description =
                "Development Modbus TCP controller.",
            .protocol =
                DeviceProtocol::ModbusTcp,
            .state = DeviceState::Active,
            .connection = DeviceConnection{
                .protocol =
                    DeviceProtocol::ModbusTcp,
                .host = "127.0.0.1",
                .port = 1502,
                .ip_mode = IpMode::IPv4,
                .network_interface = ""
            }
        },
        Device{
            .id = make_device_id(
                "device-snmp-1"
            ),
            .object_id = make_object_id(
                "building-admin"
            ),
            .code = "UPS_MAIN",
            .name = "Main UPS",
            .description =
                "Development SNMP power device.",
            .protocol = DeviceProtocol::Snmp,
            .state =
                DeviceState::Maintenance,
            .connection = DeviceConnection{
                .protocol =
                    DeviceProtocol::Snmp,
                .host = "2001:db8::10",
                .port = 161,
                .ip_mode = IpMode::IPv6,
                .network_interface = "eth0"
            }
        },
        Device{
            .id = make_device_id(
                "device-simulator-1"
            ),
            .object_id = make_object_id(
                "line-water"
            ),
            .code = "SIMULATOR_WATER",
            .name = "Water line simulator",
            .description =
                "Development process simulator.",
            .protocol =
                DeviceProtocol::Simulator,
            .state =
                DeviceState::Simulation,
            .connection = DeviceConnection{
                .protocol =
                    DeviceProtocol::Simulator,
                .host = "",
                .port = 0,
                .ip_mode = IpMode::Auto,
                .network_interface = ""
            }
        },
        Device{
            .id = make_device_id(
                "device-disabled-1"
            ),
            .object_id = make_object_id(
                "group-pumps"
            ),
            .code =
                "PUMP_CONTROLLER_DISABLED",
            .name = "Disabled pump controller",
            .description =
                "Disabled development controller.",
            .protocol =
                DeviceProtocol::ModbusTcp,
            .state = DeviceState::Disabled,
            .connection = DeviceConnection{
                .protocol =
                    DeviceProtocol::ModbusTcp,
                .host = "192.0.2.10",
                .port = 502,
                .ip_mode = IpMode::IPv4,
                .network_interface = ""
            }
        }
    };
}

[[nodiscard]] std::vector<Tag>
make_development_tags()
{
    return {
        Tag{
            .id = make_tag_id(
                "tag-temperature"
            ),
            .object_id = make_object_id(
                "room-control"
            ),
            .device_id = make_device_id(
                "device-modbus-1"
            ),
            .type = TagType::DeviceTag,
            .value_type =
                TagValueType::Double,
            .archive_policy =
                TagArchivePolicy::OnChange,
            .code = "TEMPERATURE",
            .name = "Room temperature",
            .description =
                "Control room air temperature.",
            .engineering_unit = "degC",
            .scale = 0.1,
            .offset = 0.0,
            .address = TagAddress{
                .protocol =
                    DeviceProtocol::ModbusTcp,
                .address = "40001"
            },
            .is_enabled = true
        },
        Tag{
            .id = make_tag_id(
                "tag-fan-running"
            ),
            .object_id = make_object_id(
                "room-control"
            ),
            .device_id = make_device_id(
                "device-modbus-1"
            ),
            .type = TagType::DeviceTag,
            .value_type =
                TagValueType::Boolean,
            .archive_policy =
                TagArchivePolicy::OnQualityChange,
            .code = "FAN_RUNNING",
            .name = "Ventilation fan running",
            .description =
                "Ventilation fan running state.",
            .engineering_unit = "",
            .scale = 1.0,
            .offset = 0.0,
            .address = TagAddress{
                .protocol =
                    DeviceProtocol::ModbusTcp,
                .address = "00001"
            },
            .is_enabled = true
        },
        Tag{
            .id = make_tag_id(
                "tag-pump-pressure"
            ),
            .object_id = make_object_id(
                "group-pumps"
            ),
            .device_id = make_device_id(
                "device-disabled-1"
            ),
            .type = TagType::DeviceTag,
            .value_type =
                TagValueType::Float,
            .archive_policy =
                TagArchivePolicy::Periodic,
            .code = "PUMP_PRESSURE",
            .name = "Pump pressure",
            .description =
                "Pump discharge pressure.",
            .engineering_unit = "bar",
            .scale = 0.01,
            .offset = 0.0,
            .address = TagAddress{
                .protocol =
                    DeviceProtocol::ModbusTcp,
                .address = "40010"
            },
            .is_enabled = true
        },
        Tag{
            .id = make_tag_id(
                "tag-operator-setpoint"
            ),
            .object_id = make_object_id(
                "line-water"
            ),
            .device_id = std::nullopt,
            .type = TagType::ManualTag,
            .value_type =
                TagValueType::Double,
            .archive_policy =
                TagArchivePolicy::Periodic,
            .code = "OPERATOR_SETPOINT",
            .name = "Operator setpoint",
            .description =
                "Manual development setpoint.",
            .engineering_unit = "percent",
            .scale = 1.0,
            .offset = 0.0,
            .address = TagAddress{},
            .is_enabled = true
        },
        Tag{
            .id = make_tag_id(
                "tag-system-status"
            ),
            .object_id = std::nullopt,
            .device_id = std::nullopt,
            .type = TagType::SystemTag,
            .value_type =
                TagValueType::String,
            .archive_policy =
                TagArchivePolicy::Disabled,
            .code = "SYSTEM_STATUS",
            .name = "System status",
            .description =
                "Development system status tag.",
            .engineering_unit = "",
            .scale = 1.0,
            .offset = 0.0,
            .address = TagAddress{},
            .is_enabled = true
        },
        Tag{
            .id = make_tag_id(
                "tag-valve-command-disabled"
            ),
            .object_id = make_object_id(
                "group-pumps"
            ),
            .device_id = make_device_id(
                "device-disabled-1"
            ),
            .type = TagType::CommandTag,
            .value_type =
                TagValueType::Boolean,
            .archive_policy =
                TagArchivePolicy::Disabled,
            .code =
                "VALVE_COMMAND_DISABLED",
            .name = "Disabled valve command",
            .description =
                "Disabled development command tag.",
            .engineering_unit = "",
            .scale = 1.0,
            .offset = 0.0,
            .address = TagAddress{
                .protocol =
                    DeviceProtocol::ModbusTcp,
                .address = "00002"
            },
            .is_enabled = false
        }
    };
}

[[nodiscard]] bool contains_object(
    const std::vector<ObjectNode>& objects,
    const ObjectId& id
)
{
    for (const auto& object : objects) {
        if (object.id == id) {
            return true;
        }
    }

    return false;
}

[[nodiscard]] const Device* find_device(
    const std::vector<Device>& devices,
    const DeviceId& id
)
{
    for (const auto& device : devices) {
        if (device.id == id) {
            return &device;
        }
    }

    return nullptr;
}

void validate_development_snapshot(
    const std::vector<ObjectNode>& objects,
    const std::vector<Device>& devices,
    const std::vector<Tag>& tags
)
{
    const auto object_tree =
        ObjectTree::from_objects(objects);

    if (!object_tree.validate().is_valid()) {
        throw std::logic_error{
            "Development object snapshot is invalid."
        };
    }

    std::unordered_set<std::string>
        device_ids;

    for (const auto& device : devices) {
        if (
            !device_ids.insert(
                device.id.str()
            ).second
        ) {
            throw std::logic_error{
                "Development device id is duplicated."
            };
        }

        if (
            !dispatcher::devices::validate_device(
                device
            ).is_valid()
        ) {
            throw std::logic_error{
                "Development device is invalid: "
                + device.id.str()
            };
        }

        if (
            device.object_id.has_value()
            && !contains_object(
                objects,
                device.object_id.value()
            )
        ) {
            throw std::logic_error{
                "Development device references "
                "unknown object: "
                + device.id.str()
            };
        }
    }

    std::unordered_set<std::string>
        tag_ids;

    for (const auto& tag : tags) {
        if (
            !tag_ids.insert(
                tag.id.str()
            ).second
        ) {
            throw std::logic_error{
                "Development tag id is duplicated."
            };
        }

        if (
            !dispatcher::tags::validate_tag(
                tag
            ).is_valid()
        ) {
            throw std::logic_error{
                "Development tag is invalid: "
                + tag.id.str()
            };
        }

        if (
            tag.object_id.has_value()
            && !contains_object(
                objects,
                tag.object_id.value()
            )
        ) {
            throw std::logic_error{
                "Development tag references "
                "unknown object: "
                + tag.id.str()
            };
        }

        if (tag.device_id.has_value()) {
            const auto* device =
                find_device(
                    devices,
                    tag.device_id.value()
                );

            if (device == nullptr) {
                throw std::logic_error{
                    "Development tag references "
                    "unknown device: "
                    + tag.id.str()
                };
            }

            if (
                tag.object_id.has_value()
                && device->object_id.has_value()
                && tag.object_id.value()
                    != device->object_id.value()
            ) {
                throw std::logic_error{
                    "Development tag and device "
                    "object assignments differ: "
                    + tag.id.str()
                };
            }
        }
    }
}

template <typename T>
void sort_by_code_and_id(
    std::vector<T>& values
)
{
    std::ranges::sort(
        values,
        [](const T& left, const T& right) {
            if (left.code != right.code) {
                return left.code < right.code;
            }

            return left.id < right.id;
        }
    );
}

template <typename T>
[[nodiscard]] ReadCollection<T> paginate(
    std::vector<T> values,
    const dispatcher::api::ApiReadQueryOptions& options
)
{
    ReadCollection<T> result;

    result.total =
        static_cast<std::uint64_t>(
            values.size()
        );

    result.limit =
        options.normalized_limit();

    const auto start =
        std::min<std::size_t>(
            options.offset,
            values.size()
        );

    const auto end =
        std::min<std::size_t>(
            start + result.limit,
            values.size()
        );

    result.offset =
        static_cast<std::uint32_t>(
            start
        );

    result.items.reserve(
        end - start
    );

    for (
        auto index = start;
        index < end;
        ++index
    ) {
        result.items.push_back(
            std::move(values[index])
        );
    }

    result.has_more =
        end < values.size();

    return result;
}

[[nodiscard]] bool object_matches(
    const ObjectNode& object,
    const ObjectReadQuery& query
)
{
    if (
        query.filter.has_id()
        && object.id.str()
            != query.filter.id
    ) {
        return false;
    }

    if (query.has_parent_id()) {
        if (
            !object.parent_id.has_value()
            || object.parent_id->str()
                != query.parent_id
        ) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] std::vector<const ObjectNode*>
select_objects(
    const std::vector<ObjectNode>& objects,
    const ObjectReadQuery& query
)
{
    std::vector<const ObjectNode*>
        selected;

    for (const auto& object : objects) {
        if (object_matches(object, query)) {
            selected.push_back(
                &object
            );
        }
    }

    if (
        !query.options.include_children
        || selected.empty()
    ) {
        return selected;
    }

    std::unordered_set<std::string>
        selected_ids;

    for (const auto* object : selected) {
        selected_ids.insert(
            object->id.str()
        );
    }

    bool changed = true;

    while (changed) {
        changed = false;

        for (const auto& object : objects) {
            if (
                !object.parent_id.has_value()
                || selected_ids.contains(
                    object.id.str()
                )
            ) {
                continue;
            }

            if (
                selected_ids.contains(
                    object.parent_id->str()
                )
            ) {
                selected_ids.insert(
                    object.id.str()
                );

                changed = true;
            }
        }
    }

    selected.clear();

    for (const auto& object : objects) {
        if (
            selected_ids.contains(
                object.id.str()
            )
        ) {
            selected.push_back(
                &object
            );
        }
    }

    return selected;
}

[[nodiscard]] const ObjectTreeNodeReadModel*
find_tree_node(
    const std::vector<ObjectTreeNodeReadModel>& nodes,
    const std::string_view id
)
{
    for (const auto& node : nodes) {
        if (node.id == id) {
            return &node;
        }

        const auto* nested =
            find_tree_node(
                node.children,
                id
            );

        if (nested != nullptr) {
            return nested;
        }
    }

    return nullptr;
}

[[nodiscard]] std::uint64_t count_tree_nodes(
    const ObjectTreeNodeReadModel& node
)
{
    std::uint64_t count = 1;

    for (const auto& child : node.children) {
        count += count_tree_nodes(child);
    }

    return count;
}

[[nodiscard]] bool is_hidden_device(
    const Device& device
) noexcept
{
    return device.state == DeviceState::Disabled
        || device.state
            == DeviceState::Decommissioned;
}

} // namespace

DevelopmentConfigurationReadService::
DevelopmentConfigurationReadService()
    : objects_(
        make_development_objects()
    )
    , devices_(
        make_development_devices()
    )
    , tags_(
        make_development_tags()
    )
{
    validate_development_snapshot(
        objects_,
        devices_,
        tags_
    );
}

ReadCollection<ObjectReadModel>
DevelopmentConfigurationReadService::read_objects(
    const ObjectReadQuery& query
) const
{
    const auto selected =
        select_objects(
            objects_,
            query
        );

    std::vector<ObjectReadModel>
        result_items;

    result_items.reserve(
        selected.size()
    );

    for (const auto* object : selected) {
        result_items.push_back(
            dispatcher::api::to_read_model(
                *object
            )
        );
    }

    sort_by_code_and_id(
        result_items
    );

    return paginate(
        std::move(result_items),
        query.options
    );
}

ObjectTreeReadResult
DevelopmentConfigurationReadService::read_object_tree(
    const ObjectReadQuery& query
) const
{
    const auto full_tree =
        dispatcher::api::
            build_object_tree_read_result(
                objects_
            );

    if (query.filter.has_id()) {
        const auto* selected =
            find_tree_node(
                full_tree.roots,
                query.filter.id
            );

        if (selected == nullptr) {
            return {};
        }

        if (
            query.has_parent_id()
            && (
                !selected->parent_id.has_value()
                || selected->parent_id.value()
                    != query.parent_id
            )
        ) {
            return {};
        }

        ObjectTreeReadResult result;

        result.roots.push_back(
            *selected
        );

        result.count =
            count_tree_nodes(
                *selected
            );

        return result;
    }

    if (query.has_parent_id()) {
        const auto* parent =
            find_tree_node(
                full_tree.roots,
                query.parent_id
            );

        if (parent == nullptr) {
            return {};
        }

        ObjectTreeReadResult result;

        result.roots =
            parent->children;

        for (const auto& root : result.roots) {
            result.count +=
                count_tree_nodes(root);
        }

        return result;
    }

    return full_tree;
}

ReadCollection<DeviceReadModel>
DevelopmentConfigurationReadService::read_devices(
    const DeviceReadQuery& query
) const
{
    std::vector<DeviceReadModel>
        result_items;

    for (const auto& device : devices_) {
        if (
            !query.options.include_disabled
            && is_hidden_device(device)
        ) {
            continue;
        }

        if (
            query.filter.has_id()
            && device.id.str()
                != query.filter.id
        ) {
            continue;
        }

        if (
            query.filter.has_object_id()
            && (
                !device.object_id.has_value()
                || device.object_id->str()
                    != query.filter.object_id
            )
        ) {
            continue;
        }

        if (
            query.filter.has_state()
            && dispatcher::devices::to_string(
                device.state
            ) != query.filter.state
        ) {
            continue;
        }

        result_items.push_back(
            dispatcher::api::to_read_model(
                device
            )
        );
    }

    sort_by_code_and_id(
        result_items
    );

    return paginate(
        std::move(result_items),
        query.options
    );
}

ReadCollection<TagReadModel>
DevelopmentConfigurationReadService::read_tags(
    const TagReadQuery& query
) const
{
    std::vector<TagReadModel>
        result_items;

    for (const auto& tag : tags_) {
        if (
            !query.options.include_disabled
            && !tag.is_enabled
        ) {
            continue;
        }

        if (
            query.filter.has_id()
            && tag.id.str()
                != query.filter.id
        ) {
            continue;
        }

        if (
            query.filter.has_tag_id()
            && tag.id.str()
                != query.filter.tag_id
        ) {
            continue;
        }

        if (
            query.filter.has_object_id()
            && (
                !tag.object_id.has_value()
                || tag.object_id->str()
                    != query.filter.object_id
            )
        ) {
            continue;
        }

        if (
            query.filter.has_device_id()
            && (
                !tag.device_id.has_value()
                || tag.device_id->str()
                    != query.filter.device_id
            )
        ) {
            continue;
        }

        result_items.push_back(
            dispatcher::api::to_read_model(
                tag
            )
        );
    }

    sort_by_code_and_id(
        result_items
    );

    return paginate(
        std::move(result_items),
        query.options
    );
}

std::size_t
DevelopmentConfigurationReadService::object_count() const noexcept
{
    return objects_.size();
}

std::size_t
DevelopmentConfigurationReadService::device_count() const noexcept
{
    return devices_.size();
}

std::size_t
DevelopmentConfigurationReadService::tag_count() const noexcept
{
    return tags_.size();
}

dispatcher::api::ConfigurationReadServicePtr
create_development_configuration_read_service()
{
    return std::make_shared<
        DevelopmentConfigurationReadService
    >();
}

} // namespace dispatcher::app
