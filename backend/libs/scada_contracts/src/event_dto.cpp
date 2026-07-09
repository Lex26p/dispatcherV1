#include "scada_contracts/events/event_dto.h"

namespace dispatcher::contracts::events
{
    bool EventRecordDto::has_event_id() const noexcept
    {
        return !id.empty();
    }

    bool EventRecordDto::has_code() const noexcept
    {
        return !code.empty();
    }

    bool EventRecordDto::has_title() const noexcept
    {
        return !title.empty();
    }

    bool EventRecordDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool EventRecordDto::has_source_id() const noexcept
    {
        return !source_id.empty();
    }

    bool EventRecordDto::has_object_id() const noexcept
    {
        return !object_id.empty();
    }

    bool EventRecordDto::has_device_id() const noexcept
    {
        return !device_id.empty();
    }

    bool EventRecordDto::has_tag_id() const noexcept
    {
        return !tag_id.empty();
    }

    bool EventRecordDto::has_correlation_id() const noexcept
    {
        return !correlation_id.empty();
    }

    bool EventRecordDto::has_timestamp() const noexcept
    {
        return !timestamp.empty();
    }

    bool EventListDto::empty() const noexcept
    {
        return events.empty();
    }

    std::uint64_t EventListDto::size() const noexcept
    {
        return static_cast<std::uint64_t>(events.size());
    }

    bool EventListDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool EventValidationIssueDto::has_message() const noexcept
    {
        return !message.empty();
    }

    bool EventValidationResultDto::is_valid() const noexcept
    {
        return issues.empty();
    }

    bool EventValidationResultDto::has_issues() const noexcept
    {
        return !issues.empty();
    }
}