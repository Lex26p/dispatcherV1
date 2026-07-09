#pragma once

#include "scada_devices/device_ids.h"
#include "scada_devices/device_protocol.h"
#include "scada_polling/polling_group.h"
#include "scada_polling/polling_group_mode.h"
#include "scada_polling/polling_ids.h"

#include <optional>
#include <string_view>
#include <vector>

namespace dispatcher::polling::repository
{
    class IPollingGroupRepository
    {
    public:
        virtual ~IPollingGroupRepository() = default;

        [[nodiscard]] virtual std::vector<PollingGroup> get_all() const = 0;

        [[nodiscard]] virtual std::optional<PollingGroup> find_by_id(
            const PollingGroupId& id
        ) const = 0;

        [[nodiscard]] virtual std::optional<PollingGroup> find_by_code(
            std::string_view code
        ) const = 0;

        [[nodiscard]] virtual std::vector<PollingGroup> find_by_device_id(
            const dispatcher::devices::DeviceId& device_id
        ) const = 0;

        [[nodiscard]] virtual std::vector<PollingGroup> find_by_protocol(
            dispatcher::devices::DeviceProtocol protocol
        ) const = 0;

        [[nodiscard]] virtual std::vector<PollingGroup> find_by_mode(
            PollingGroupMode mode
        ) const = 0;

        virtual void save(
            const PollingGroup& group
        ) = 0;

        virtual bool remove_by_id(
            const PollingGroupId& id
        ) = 0;
    };
}