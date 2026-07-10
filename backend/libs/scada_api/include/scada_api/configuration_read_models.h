#pragma once

#include "scada_devices/device.h"
#include "scada_objects/object_node.h"
#include "scada_tags/tag.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::api {

struct ObjectReadModel {
    std::string id;
    std::optional<std::string> parent_id;

    std::string type;
    std::string code;
    std::string name;
    std::string description;

    bool is_root = false;
};

struct ObjectTreeNodeReadModel {
    std::string id;
    std::optional<std::string> parent_id;

    std::string type;
    std::string code;
    std::string name;
    std::string description;

    std::vector<ObjectTreeNodeReadModel> children;

    [[nodiscard]] bool has_children() const noexcept;
};

struct DeviceConnectionReadModel {
    std::string host;
    std::uint16_t port = 0;

    std::string ip_mode;
    std::string network_interface;
};

struct DeviceReadModel {
    std::string id;
    std::optional<std::string> object_id;

    std::string code;
    std::string name;
    std::string description;

    std::string protocol;
    std::string state;

    bool runtime_enabled = false;

    DeviceConnectionReadModel connection;
};

struct TagReadModel {
    std::string id;

    std::optional<std::string> object_id;
    std::optional<std::string> device_id;

    std::string type;
    std::string value_type;
    std::string archive_policy;

    std::string code;
    std::string name;
    std::string description;

    std::string engineering_unit;

    double scale = 1.0;
    double offset = 0.0;

    std::string address;

    bool enabled = true;
};

template <typename T>
struct ReadCollection {
    std::vector<T> items;

    std::uint64_t total = 0;

    std::uint32_t offset = 0;
    std::uint32_t limit = 0;

    bool has_more = false;

    [[nodiscard]] std::size_t count() const noexcept
    {
        return items.size();
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return items.empty();
    }
};

struct ObjectTreeReadResult {
    std::vector<ObjectTreeNodeReadModel> roots;

    std::uint64_t count = 0;

    [[nodiscard]] bool empty() const noexcept;
};

[[nodiscard]] ObjectReadModel to_read_model(
    const dispatcher::objects::ObjectNode& object
);

[[nodiscard]] DeviceReadModel to_read_model(
    const dispatcher::devices::Device& device
);

[[nodiscard]] TagReadModel to_read_model(
    const dispatcher::tags::Tag& tag
);

[[nodiscard]] ObjectTreeReadResult build_object_tree_read_result(
    const std::vector<dispatcher::objects::ObjectNode>& objects
);

} // namespace dispatcher::api
