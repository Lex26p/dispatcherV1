#pragma once

#include "scada_devices/device.h"
#include "scada_devices/device_ids.h"
#include "scada_devices/device_protocol.h"
#include "scada_devices/device_state.h"
#include "scada_objects/object_ids.h"

#include <optional>
#include <string_view>
#include <vector>

namespace dispatcher::devices::repository
{
    class IDeviceRepository
    {
    public:
        virtual ~IDeviceRepository() = default;

        [[nodiscard]] virtual std::vector<Device> get_all() const = 0;

        [[nodiscard]] virtual std::optional<Device> find_by_id(
            const DeviceId& id
        ) const = 0;

        [[nodiscard]] virtual std::optional<Device> find_by_code(
            std::string_view code
        ) const = 0;

        [[nodiscard]] virtual std::vector<Device> find_by_object_id(
            const dispatcher::objects::ObjectId& object_id
        ) const = 0;

        [[nodiscard]] virtual std::vector<Device> find_by_protocol(
            DeviceProtocol protocol
        ) const = 0;

        [[nodiscard]] virtual std::vector<Device> find_by_state(
            DeviceState state
        ) const = 0;

        virtual void save(
            const Device& device
        ) = 0;

        virtual bool remove_by_id(
            const DeviceId& id
        ) = 0;
    };
}