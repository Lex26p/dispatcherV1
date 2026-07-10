#pragma once

#include "scada_historian/history_batch.h"
#include "scada_historian/history_ids.h"
#include "scada_historian/history_query.h"
#include "scada_historian/history_sample.h"
#include "scada_tags/tag_ids.h"

#include <cstddef>
#include <optional>
#include <vector>
#include <cstdint>

namespace dispatcher::historian::repository
{
    class IHistorySampleRepository
    {
    public:
        virtual ~IHistorySampleRepository() = default;

        [[nodiscard]] virtual std::optional<HistorySample> find_by_id(
            const HistorySampleId& id
        ) const = 0;

        [[nodiscard]] virtual std::optional<HistorySample> find_latest_by_tag_id(
            const dispatcher::tags::TagId& tag_id
        ) const = 0;

        [[nodiscard]] virtual HistoryQueryResult query(
            const HistoryQuery& query
        ) const = 0;

        [[nodiscard]] virtual std::vector<HistorySample> find_by_tag_id(
            const dispatcher::tags::TagId& tag_id,
            std::size_t limit
        ) const = 0;

        virtual void save(
            const HistorySample& sample
        ) = 0;

        [[nodiscard]] virtual HistoryBatchWriteResult save_batch(
            const HistoryBatch& batch
        ) = 0;

        virtual bool remove_by_id(
            const HistorySampleId& id
        ) = 0;

        virtual std::uint64_t remove_by_tag_id(
            const dispatcher::tags::TagId& tag_id
        ) = 0;

        virtual std::uint64_t remove_older_than(
            dispatcher::tags::TagTimestamp timestamp
        ) = 0;
    };
}
