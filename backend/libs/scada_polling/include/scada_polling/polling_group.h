#pragma once

#include "scada_devices/device_ids.h"
#include "scada_devices/device_protocol.h"
#include "scada_polling/polling_group_mode.h"
#include "scada_polling/polling_ids.h"
#include "scada_polling/polling_task.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dispatcher::polling
{
    struct PollingGroup
    {
        PollingGroupId id;

        dispatcher::devices::DeviceId device_id;
        dispatcher::devices::DeviceProtocol protocol = dispatcher::devices::DeviceProtocol::Simulator;

        PollingGroupMode mode = PollingGroupMode::Batch;

        std::string code;
        std::string name;
        std::string description;

        std::uint32_t interval_ms = 1000;
        std::uint32_t timeout_ms = 1000;
        std::uint32_t max_batch_size = 64;
        std::uint32_t priority = 100;

        bool is_enabled = true;

        std::vector<PollingTask> tasks;

        [[nodiscard]] bool has_valid_identity() const noexcept;
        [[nodiscard]] bool has_valid_timing() const noexcept;
        [[nodiscard]] bool has_tasks() const noexcept;
        [[nodiscard]] std::size_t task_count() const noexcept;
        [[nodiscard]] std::size_t enabled_task_count() const noexcept;
        [[nodiscard]] bool is_enabled_for_polling() const noexcept;
        [[nodiscard]] bool can_use_batch_mode() const noexcept;
    };
}
