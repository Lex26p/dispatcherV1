#pragma once

#include "scada_alarms/alarm_ids.h"
#include "scada_alarms/alarm_lifecycle.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace dispatcher::alarms::repository
{
    class IAlarmTransitionRepository
    {
    public:
        virtual ~IAlarmTransitionRepository() = default;

        [[nodiscard]] virtual std::vector<AlarmTransitionRecord> find_by_alarm_id(
            const AlarmId& alarm_id,
            std::size_t limit
        ) const = 0;

        virtual void save(
            const AlarmId& alarm_id,
            const AlarmTransitionRecord& transition
        ) = 0;

        virtual std::uint64_t remove_by_alarm_id(
            const AlarmId& alarm_id
        ) = 0;
    };
}
