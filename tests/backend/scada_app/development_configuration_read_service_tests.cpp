#include "scada_app/development_configuration_read_service.h"

#include "scada_api/configuration_read_service.h"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <string>

namespace {

using dispatcher::api::DeviceReadQuery;
using dispatcher::api::ObjectReadQuery;
using dispatcher::api::TagReadQuery;

using dispatcher::app::
    DevelopmentConfigurationReadService;

[[nodiscard]] bool contains_id(
    const auto& values,
    const std::string& id
)
{
    return std::ranges::any_of(
        values,
        [&id](const auto& value) {
            return value.id == id;
        }
    );
}

} // namespace

TEST_CASE(
    "Development configuration snapshot is valid and deterministic",
    "[scada_app][configuration][snapshot]"
)
{
    const DevelopmentConfigurationReadService
        service;

    REQUIRE(service.object_count() == 5);
    REQUIRE(service.device_count() == 4);
    REQUIRE(service.tag_count() == 6);

    const auto objects =
        service.read_objects(
            ObjectReadQuery{}
        );

    const auto devices =
        service.read_devices(
            DeviceReadQuery{}
        );

    const auto tags =
        service.read_tags(
            TagReadQuery{}
        );

    REQUIRE(objects.total == 5);
    REQUIRE(devices.total == 3);
    REQUIRE(tags.total == 5);
}

TEST_CASE(
    "Object reads are ordered before pagination",
    "[scada_app][configuration][objects]"
)
{
    const DevelopmentConfigurationReadService
        service;

    ObjectReadQuery query;

    query.options.limit = 2;
    query.options.offset = 1;

    const auto result =
        service.read_objects(query);

    REQUIRE(result.total == 5);
    REQUIRE(result.count() == 2);
    REQUIRE(result.offset == 1);
    REQUIRE(result.limit == 2);
    REQUIRE(result.has_more);

    REQUIRE(
        result.items[0].code
        == "CONTROL_ROOM"
    );

    REQUIRE(
        result.items[1].code
        == "PUMP_GROUP"
    );
}

TEST_CASE(
    "Object read includes descendants when requested",
    "[scada_app][configuration][objects]"
)
{
    const DevelopmentConfigurationReadService
        service;

    ObjectReadQuery query;

    query.filter.id = "site-main";
    query.options.include_children = true;

    const auto result =
        service.read_objects(query);

    REQUIRE(result.total == 3);

    REQUIRE(
        contains_id(
            result.items,
            "site-main"
        )
    );

    REQUIRE(
        contains_id(
            result.items,
            "building-admin"
        )
    );

    REQUIRE(
        contains_id(
            result.items,
            "room-control"
        )
    );
}

TEST_CASE(
    "Object tree can return selected subtree",
    "[scada_app][configuration][tree]"
)
{
    const DevelopmentConfigurationReadService
        service;

    const auto full_tree =
        service.read_object_tree(
            ObjectReadQuery{}
        );

    REQUIRE(full_tree.count == 5);
    REQUIRE(full_tree.roots.size() == 2);

    ObjectReadQuery query;

    query.filter.id = "site-main";

    const auto subtree =
        service.read_object_tree(query);

    REQUIRE(subtree.count == 3);
    REQUIRE(subtree.roots.size() == 1);

    REQUIRE(
        subtree.roots[0].id
        == "site-main"
    );

    REQUIRE(
        subtree.roots[0].children[0].id
        == "building-admin"
    );
}

TEST_CASE(
    "Device reads filter disabled and object assignment",
    "[scada_app][configuration][devices]"
)
{
    const DevelopmentConfigurationReadService
        service;

    const auto default_result =
        service.read_devices(
            DeviceReadQuery{}
        );

    REQUIRE(default_result.total == 3);

    DeviceReadQuery include_disabled;

    include_disabled.options.include_disabled =
        true;

    const auto all_devices =
        service.read_devices(
            include_disabled
        );

    REQUIRE(all_devices.total == 4);

    REQUIRE(
        contains_id(
            all_devices.items,
            "device-disabled-1"
        )
    );

    DeviceReadQuery by_object;

    by_object.filter.object_id =
        "room-control";

    const auto room_devices =
        service.read_devices(by_object);

    REQUIRE(room_devices.total == 1);

    REQUIRE(
        room_devices.items[0].id
        == "device-modbus-1"
    );
}

TEST_CASE(
    "Tag reads filter disabled and device assignment",
    "[scada_app][configuration][tags]"
)
{
    const DevelopmentConfigurationReadService
        service;

    const auto default_result =
        service.read_tags(
            TagReadQuery{}
        );

    REQUIRE(default_result.total == 5);

    TagReadQuery query;

    query.options.include_disabled = true;
    query.filter.device_id =
        "device-disabled-1";

    const auto device_tags =
        service.read_tags(query);

    REQUIRE(device_tags.total == 2);

    REQUIRE(
        contains_id(
            device_tags.items,
            "tag-pump-pressure"
        )
    );

    REQUIRE(
        contains_id(
            device_tags.items,
            "tag-valve-command-disabled"
        )
    );
}

TEST_CASE(
    "Development configuration service factory returns readable service",
    "[scada_app][configuration][factory]"
)
{
    const auto service =
        dispatcher::app::
            create_development_configuration_read_service();

    REQUIRE(service != nullptr);

    const auto objects =
        service->read_objects(
            ObjectReadQuery{}
        );

    REQUIRE(objects.total == 5);
    REQUIRE_FALSE(objects.empty());
}
