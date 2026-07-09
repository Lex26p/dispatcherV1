#pragma once

#include "scada_events/event_ids.h"
#include "scada_events/event_record.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dispatcher::events::repository
{
    class IEventRecordRepository
    {
    public:
        virtual ~IEventRecordRepository() = default;

        [[nodiscard]] virtual std::optional<EventRecord> find_by_id(
            const EventId& id
        ) const = 0;

        [[nodiscard]] virtual std::vector<EventRecord> find_recent(
            std::size_t limit
        ) const = 0;

        [[nodiscard]] virtual std::vector<EventRecord> find_by_category(
            EventCategory category,
            std::size_t limit
        ) const = 0;

        [[nodiscard]] virtual std::vector<EventRecord> find_by_source_id(
            const std::string& source_id,
            std::size_t limit
        ) const = 0;

        [[nodiscard]] virtual std::vector<EventRecord> find_by_correlation_id(
            const std::string& correlation_id,
            std::size_t limit
        ) const = 0;

        virtual void save(
            const EventRecord& event
        ) = 0;

        virtual bool remove_by_id(
            const EventId& id
        ) = 0;

        virtual std::uint64_t remove_older_than(
            EventTimestamp timestamp
        ) = 0;

        [[nodiscard]] virtual std::uint64_t count_all() const = 0;
    };
}