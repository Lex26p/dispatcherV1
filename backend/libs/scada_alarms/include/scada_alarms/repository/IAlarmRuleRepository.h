#pragma once

#include "scada_alarms/alarm_ids.h"
#include "scada_alarms/alarm_rule.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::alarms::repository
{
    class IAlarmRuleRepository
    {
    public:
        virtual ~IAlarmRuleRepository() = default;

        [[nodiscard]] virtual std::optional<AlarmRule> find_by_id(
            const AlarmRuleId& id
        ) const = 0;

        [[nodiscard]] virtual std::vector<AlarmRule> find_enabled(
            std::size_t limit
        ) const = 0;

        [[nodiscard]] virtual std::vector<AlarmRule> find_by_tag_id(
            const std::string& tag_id,
            std::size_t limit
        ) const = 0;

        [[nodiscard]] virtual std::vector<AlarmRule> find_by_state(
            AlarmRuleState state,
            std::size_t limit
        ) const = 0;

        virtual void save(
            const AlarmRule& rule
        ) = 0;

        virtual bool remove_by_id(
            const AlarmRuleId& id
        ) = 0;

        virtual bool set_state(
            const AlarmRuleId& id,
            AlarmRuleState state
        ) = 0;

        [[nodiscard]] virtual std::uint64_t count_enabled() const = 0;
    };
}
