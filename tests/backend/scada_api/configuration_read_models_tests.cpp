#include "scada_api/configuration_read_models.h"

#include "scada_devices/device.h"
#include "scada_objects/object_node.h"
#include "scada_tags/tag.h"

#include <catch2/catch_test_macros.hpp>

#include <optional>
#include <string>
#include <vector>

namespace {

using dispatcher::api::build_object_tree_read_result;
using dispatcher::api::to_read_model;

using dispatcher::devices::Device;
using dispatcher::devices::DeviceConnection;
using dispatcher::devices::DeviceId;
using dispatcher::devices::DeviceProtocol;
using dispatcher::devices::DeviceState;
using dispatcher::devices::IpMode;

using dispatcher::objects::ObjectId;
using dispatcher::objects::ObjectNode;
using dispatcher::objects::ObjectType;

using dispatcher::tags::Tag;
using dispatcher::tags::TagAddress;
using dispatcher::tags::TagArchivePolicy;
using dispatcher::tags::TagId;
using dispatcher::tags::TagType;
using dispatcher::tags::TagValueType;

} // namespace

TEST_CASE(
    "Object domain entity maps to read model",
    "[scada_api][configuration][mapping]"
)
{
    const ObjectNode object{
        .id = ObjectId::from_string(
            "object-1"
        ),
        .parent_id = std::nullopt,
        .type = ObjectType::Site,
        .code = "SITE_1",
        .name = "Site 1",
        .description = "Development site"
    };

    const auto model =
        to_read_model(object);

    REQUIRE(model.id == "object-1");
    REQUIRE_FALSE(model.parent_id.has_value());
    REQUIRE(model.type == "Site");
    REQUIRE(model.code == "SITE_1");
    REQUIRE(model.name == "Site 1");
    REQUIRE(model.is_root);
}

TEST_CASE(
    "Device domain entity maps connection and runtime state",
    "[scada_api][configuration][mapping]"
)
{
    const Device device{
        .id = DeviceId::from_string(
            "device-1"
        ),
        .object_id = ObjectId::from_string(
            "object-1"
        ),
        .code = "DEVICE_1",
        .name = "Device 1",
        .description = "Development device",
        .protocol =
            DeviceProtocol::ModbusTcp,
        .state = DeviceState::Active,
        .connection = DeviceConnection{
            .protocol =
                DeviceProtocol::ModbusTcp,
            .host = "127.0.0.1",
            .port = 502,
            .ip_mode = IpMode::IPv4,
            .network_interface = "eth0"
        }
    };

    const auto model =
        to_read_model(device);

    REQUIRE(model.id == "device-1");
    REQUIRE(model.object_id == "object-1");
    REQUIRE(model.protocol == "ModbusTcp");
    REQUIRE(model.state == "Active");
    REQUIRE(model.runtime_enabled);
    REQUIRE(model.connection.host == "127.0.0.1");
    REQUIRE(model.connection.port == 502);
    REQUIRE(model.connection.ip_mode == "IPv4");

    REQUIRE(
        model.connection.network_interface
        == "eth0"
    );
}

TEST_CASE(
    "Tag domain entity maps engineering configuration",
    "[scada_api][configuration][mapping]"
)
{
    const Tag tag{
        .id = TagId::from_string(
            "tag-1"
        ),
        .object_id = ObjectId::from_string(
            "object-1"
        ),
        .device_id = DeviceId::from_string(
            "device-1"
        ),
        .type = TagType::DeviceTag,
        .value_type =
            TagValueType::Double,
        .archive_policy =
            TagArchivePolicy::OnChange,
        .code = "TEMPERATURE",
        .name = "Temperature",
        .description = "Temperature tag",
        .engineering_unit = "degC",
        .scale = 0.1,
        .offset = -10.0,
        .address = TagAddress{
            .protocol =
                DeviceProtocol::ModbusTcp,
            .address = "40001"
        },
        .is_enabled = true
    };

    const auto model =
        to_read_model(tag);

    REQUIRE(model.id == "tag-1");
    REQUIRE(model.object_id == "object-1");
    REQUIRE(model.device_id == "device-1");
    REQUIRE(model.type == "DeviceTag");
    REQUIRE(model.value_type == "Double");
    REQUIRE(model.archive_policy == "OnChange");
    REQUIRE(model.engineering_unit == "degC");
    REQUIRE(model.scale == 0.1);
    REQUIRE(model.offset == -10.0);
    REQUIRE(model.address == "40001");
    REQUIRE(model.enabled);
}

TEST_CASE(
    "Object tree read model has deterministic hierarchy order",
    "[scada_api][configuration][tree]"
)
{
    const std::vector<ObjectNode> objects{
        ObjectNode{
            .id = ObjectId::from_string(
                "room-1"
            ),
            .parent_id =
                ObjectId::from_string(
                    "building-1"
                ),
            .type = ObjectType::Room,
            .code = "ROOM_1",
            .name = "Room 1",
            .description = ""
        },
        ObjectNode{
            .id = ObjectId::from_string(
                "site-1"
            ),
            .parent_id = std::nullopt,
            .type = ObjectType::Site,
            .code = "SITE_1",
            .name = "Site 1",
            .description = ""
        },
        ObjectNode{
            .id = ObjectId::from_string(
                "building-1"
            ),
            .parent_id =
                ObjectId::from_string(
                    "site-1"
                ),
            .type = ObjectType::Building,
            .code = "BUILDING_1",
            .name = "Building 1",
            .description = ""
        }
    };

    const auto tree =
        build_object_tree_read_result(
            objects
        );

    REQUIRE(tree.count == 3);
    REQUIRE(tree.roots.size() == 1);
    REQUIRE(tree.roots[0].id == "site-1");
    REQUIRE(tree.roots[0].has_children());

    REQUIRE(
        tree.roots[0].children[0].id
        == "building-1"
    );

    REQUIRE(
        tree.roots[0]
            .children[0]
            .children[0]
            .id
        == "room-1"
    );
}
