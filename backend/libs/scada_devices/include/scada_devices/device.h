#pragma once

#include "scada_devices/device_connection.h"
#include "scada_devices/device_ids.h"
#include "scada_devices/device_state.h"
#include "scada_objects/object_ids.h"

#include <optional>
#include <string>

namespace dispatcher::devices
{
    struct Device
    {
        DeviceId id;
        std::optional<dispatcher::objects::ObjectId> object_id;

        std::string code;
        std::string name;
        std::string description;

        DeviceProtocol protocol = DeviceProtocol::Custom;
        DeviceState state = DeviceState::Draft;
        DeviceConnection connection;

        [[nodiscard]] bool has_valid_identity() const noexcept;
        [[nodiscard]] bool is_assigned_to_object() const noexcept;
        [[nodiscard]] bool is_enabled_for_runtime() const noexcept;
    };
}