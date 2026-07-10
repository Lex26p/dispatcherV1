#pragma once

#include "scada_devices/device_ids.h"
#include "scada_devices/device_protocol.h"
#include "scada_polling/polling_ids.h"
#include "scada_polling/polling_task_state.h"
#include "scada_tags/tag_address.h"
#include "scada_tags/tag_ids.h"

#include <cstdint>
#include <string>

namespace dispatcher::polling
{
    struct PollingTask
    {
        PollingTaskId id;
        PollingGroupId group_id;

        dispatcher::devices::DeviceId device_id;
        dispatcher::tags::TagId tag_id;

        dispatcher::devices::DeviceProtocol protocol = dispatcher::devices::DeviceProtocol::Simulator;
        dispatcher::tags::TagAddress address;

        PollingTaskState state = PollingTaskState::Waiting;

        std::uint32_t interval_ms = 1000;
        std::uint32_t timeout_ms = 1000;
        std::uint32_t priority = 100;

        bool is_enabled = true;

        std::string description;

        [[nodiscard]] bool has_valid_identity() const noexcept;
        [[nodiscard]] bool has_valid_timing() const noexcept;
        [[nodiscard]] bool has_address() const noexcept;
        [[nodiscard]] bool is_enabled_for_polling() const noexcept;
        [[nodiscard]] bool is_batch_compatible_with(
            const PollingTask& other
        ) const noexcept;
    };
}
