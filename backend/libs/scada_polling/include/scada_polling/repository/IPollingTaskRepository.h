#pragma once

#include "scada_devices/device_ids.h"
#include "scada_polling/polling_ids.h"
#include "scada_polling/polling_task.h"
#include "scada_polling/polling_task_state.h"
#include "scada_tags/tag_ids.h"

#include <optional>
#include <vector>

namespace dispatcher::polling::repository
{
    class IPollingTaskRepository
    {
    public:
        virtual ~IPollingTaskRepository() = default;

        [[nodiscard]] virtual std::vector<PollingTask> get_all() const = 0;

        [[nodiscard]] virtual std::optional<PollingTask> find_by_id(
            const PollingTaskId& id
        ) const = 0;

        [[nodiscard]] virtual std::vector<PollingTask> find_by_group_id(
            const PollingGroupId& group_id
        ) const = 0;

        [[nodiscard]] virtual std::vector<PollingTask> find_by_device_id(
            const dispatcher::devices::DeviceId& device_id
        ) const = 0;

        [[nodiscard]] virtual std::vector<PollingTask> find_by_tag_id(
            const dispatcher::tags::TagId& tag_id
        ) const = 0;

        [[nodiscard]] virtual std::vector<PollingTask> find_by_state(
            PollingTaskState state
        ) const = 0;

        virtual void save(
            const PollingTask& task
        ) = 0;

        virtual bool remove_by_id(
            const PollingTaskId& id
        ) = 0;
    };
}
