#pragma once

#include "scada_alarms/alarm_ids.h"
#include "scada_alarms/alarm_record.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::alarms::repository
{
    class IAlarmRepository
    {
    public:
        virtual ~IAlarmRepository() = default;

        [[nodiscard]] virtual std::optional<AlarmRecord> find_by_id(
            const AlarmId& id
        ) const = 0;

        [[nodiscard]] virtual std::optional<AlarmRecord> find_active_by_source_id(
            const std::string& source_id
        ) const = 0;

        [[nodiscard]] virtual std::optional<AlarmRecord> find_active_by_tag_id(
            const std::string& tag_id
        ) const = 0;

        [[nodiscard]] virtual std::vector<AlarmRecord> find_active(
            std::size_t limit
        ) const = 0;

        [[nodiscard]] virtual std::vector<AlarmRecord> find_recent(
            std::size_t limit
        ) const = 0;

        [[nodiscard]] virtual std::vector<AlarmRecord> find_by_state(
            AlarmState state,
            std::size_t limit
        ) const = 0;

        virtual void save(
            const AlarmRecord& alarm
        ) = 0;

        virtual bool remove_by_id(
            const AlarmId& id
        ) = 0;

        virtual std::uint64_t remove_closed_older_than(
            AlarmTimestamp timestamp
        ) = 0;

        [[nodiscard]] virtual std::uint64_t count_active() const = 0;
    };
}