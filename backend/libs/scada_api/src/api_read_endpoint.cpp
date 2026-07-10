#include "scada_api/api_read_endpoint.h"

#include <algorithm>
#include <utility>

namespace dispatcher::api
{
    std::uint32_t ApiReadQueryOptions::normalized_limit() const noexcept
    {
        const auto effective_max_limit = max_limit == 0
            ? 1U
            : max_limit;

        if (limit == 0)
        {
            return 1U;
        }

        return limit > effective_max_limit
            ? effective_max_limit
            : limit;
    }

    bool ApiReadQueryOptions::has_offset() const noexcept
    {
        return offset > 0;
    }

    bool ApiReadFilter::has_id() const noexcept
    {
        return !id.empty();
    }

    bool ApiReadFilter::has_object_id() const noexcept
    {
        return !object_id.empty();
    }

    bool ApiReadFilter::has_device_id() const noexcept
    {
        return !device_id.empty();
    }

    bool ApiReadFilter::has_tag_id() const noexcept
    {
        return !tag_id.empty();
    }

    bool ApiReadFilter::has_source_id() const noexcept
    {
        return !source_id.empty();
    }

    bool ApiReadFilter::has_correlation_id() const noexcept
    {
        return !correlation_id.empty();
    }

    bool ApiReadFilter::has_state() const noexcept
    {
        return !state.empty();
    }

    bool ApiReadFilter::has_quality() const noexcept
    {
        return !quality.empty();
    }

    bool ApiReadFilter::has_severity() const noexcept
    {
        return !severity.empty();
    }

    bool ApiReadFilter::has_priority() const noexcept
    {
        return !priority.empty();
    }

    bool ApiReadFilter::has_time_range() const noexcept
    {
        return !from_timestamp.empty() ||
            !to_timestamp.empty();
    }

    bool ApiReadFilter::has_any_filter() const noexcept
    {
        return has_id() ||
            has_object_id() ||
            has_device_id() ||
            has_tag_id() ||
            has_source_id() ||
            has_correlation_id() ||
            has_state() ||
            has_quality() ||
            has_severity() ||
            has_priority() ||
            has_time_range();
    }

    bool ApiReadEndpointDefinition::has_endpoint() const noexcept
    {
        return endpoint.is_valid();
    }

    bool ApiReadEndpointDefinition::is_valid() const noexcept
    {
        return resource_type != ApiReadResourceType::Unknown &&
            has_endpoint();
    }

    bool ApiReadEndpointCatalog::empty() const noexcept
    {
        return definitions_.empty();
    }

    std::size_t ApiReadEndpointCatalog::size() const noexcept
    {
        return definitions_.size();
    }

    const std::vector<ApiReadEndpointDefinition>& ApiReadEndpointCatalog::definitions() const noexcept
    {
        return definitions_;
    }

    bool ApiReadEndpointCatalog::register_definition(
        const ApiReadEndpointDefinition& definition
    )
    {
        if (!definition.is_valid())
        {
            return false;
        }

        if (contains_path(definition.endpoint.method, definition.endpoint.path))
        {
            return false;
        }

        definitions_.push_back(definition);

        return true;
    }

    bool ApiReadEndpointCatalog::register_definition(
        ApiReadEndpointDefinition&& definition
    )
    {
        if (!definition.is_valid())
        {
            return false;
        }

        if (contains_path(definition.endpoint.method, definition.endpoint.path))
        {
            return false;
        }

        definitions_.push_back(std::move(definition));

        return true;
    }

    std::vector<ApiReadEndpointDefinition> ApiReadEndpointCatalog::find_by_resource_type(
        ApiReadResourceType resource_type
    ) const
    {
        std::vector<ApiReadEndpointDefinition> result;

        for (const auto& definition : definitions_)
        {
            if (definition.resource_type == resource_type)
            {
                result.push_back(definition);
            }
        }

        return result;
    }

    std::vector<ApiReadEndpointDefinition> ApiReadEndpointCatalog::find_by_area(
        ApiEndpointArea area
    ) const
    {
        std::vector<ApiReadEndpointDefinition> result;

        for (const auto& definition : definitions_)
        {
            if (definition.endpoint.area == area)
            {
                result.push_back(definition);
            }
        }

        return result;
    }

    void ApiReadEndpointCatalog::clear() noexcept
    {
        definitions_.clear();
    }

    bool ApiReadEndpointCatalog::contains_path(
        ApiHttpMethod method,
        const std::string& path
    ) const
    {
        return std::any_of(
            definitions_.begin(),
            definitions_.end(),
            [&](const ApiReadEndpointDefinition& definition)
            {
                return definition.endpoint.method == method &&
                    definition.endpoint.path == path;
            }
        );
    }

    ApiReadEndpointDefinition make_api_read_endpoint_definition(
        ApiReadResourceType resource_type,
        ApiReadEndpointKind kind,
        ApiEndpoint endpoint
    )
    {
        ApiReadEndpointDefinition definition;

        definition.resource_type = resource_type;
        definition.kind = kind;
        definition.endpoint = std::move(endpoint);

        return definition;
    }

    ApiReadEndpointCatalog create_default_api_read_endpoint_catalog()
    {
        ApiReadEndpointCatalog catalog;

        auto system_health = make_api_read_endpoint_definition(
            ApiReadResourceType::SystemHealth,
            ApiReadEndpointKind::SingleResource,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::System,
                "/api/system/health",
                "System health",
                "Returns basic system health information."
            )
        );
        system_health.supports_filtering = false;
        system_health.supports_pagination = false;
        system_health.returns_collection = false;
        catalog.register_definition(std::move(system_health));

        auto system_modules = make_api_read_endpoint_definition(
            ApiReadResourceType::SystemModules,
            ApiReadEndpointKind::Collection,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::System,
                "/api/system/modules",
                "System modules",
                "Returns registered backend modules."
            )
        );
        system_modules.supports_filtering = false;
        system_modules.supports_pagination = false;
        catalog.register_definition(std::move(system_modules));

        auto object_tree = make_api_read_endpoint_definition(
            ApiReadResourceType::ObjectTree,
            ApiReadEndpointKind::Tree,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Objects,
                "/api/objects/tree",
                "Object tree",
                "Returns object hierarchy tree."
            )
        );
        object_tree.supports_filtering = true;
        object_tree.supports_pagination = false;
        object_tree.returns_collection = true;
        catalog.register_definition(std::move(object_tree));

        auto objects = make_api_read_endpoint_definition(
            ApiReadResourceType::Objects,
            ApiReadEndpointKind::Collection,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Objects,
                "/api/objects",
                "Objects",
                "Returns objects."
            )
        );
        objects.supports_filtering = true;
        catalog.register_definition(std::move(objects));

        auto devices = make_api_read_endpoint_definition(
            ApiReadResourceType::Devices,
            ApiReadEndpointKind::Collection,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Devices,
                "/api/devices",
                "Devices",
                "Returns devices."
            )
        );
        devices.supports_filtering = true;
        catalog.register_definition(std::move(devices));

        auto tags = make_api_read_endpoint_definition(
            ApiReadResourceType::Tags,
            ApiReadEndpointKind::Collection,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Tags,
                "/api/tags",
                "Tags",
                "Returns tags."
            )
        );
        tags.supports_filtering = true;
        catalog.register_definition(std::move(tags));

        auto runtime_values = make_api_read_endpoint_definition(
            ApiReadResourceType::RuntimeValues,
            ApiReadEndpointKind::Collection,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Runtime,
                "/api/runtime/values",
                "Runtime values",
                "Returns current runtime tag values."
            )
        );
        runtime_values.supports_filtering = true;
        runtime_values.supports_realtime = true;
        catalog.register_definition(std::move(runtime_values));

        auto history_samples = make_api_read_endpoint_definition(
            ApiReadResourceType::HistorySamples,
            ApiReadEndpointKind::Query,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Historian,
                "/api/history/query",
                "History query",
                "Returns historical tag values."
            )
        );
        history_samples.supports_filtering = true;
        history_samples.supports_time_range = true;
        catalog.register_definition(std::move(history_samples));

        auto events = make_api_read_endpoint_definition(
            ApiReadResourceType::Events,
            ApiReadEndpointKind::Collection,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Events,
                "/api/events",
                "Events",
                "Returns event records."
            )
        );
        events.supports_filtering = true;
        events.supports_time_range = true;
        catalog.register_definition(std::move(events));

        auto active_alarms = make_api_read_endpoint_definition(
            ApiReadResourceType::ActiveAlarms,
            ApiReadEndpointKind::Collection,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Alarms,
                "/api/alarms/active",
                "Active alarms",
                "Returns active alarms."
            )
        );
        active_alarms.supports_filtering = true;
        active_alarms.supports_realtime = true;
        catalog.register_definition(std::move(active_alarms));

        auto alarm_rules = make_api_read_endpoint_definition(
            ApiReadResourceType::AlarmRules,
            ApiReadEndpointKind::Collection,
            make_api_endpoint(
                ApiHttpMethod::Get,
                ApiEndpointArea::Alarms,
                "/api/alarms/rules",
                "Alarm rules",
                "Returns alarm rules."
            )
        );
        alarm_rules.supports_filtering = true;
        catalog.register_definition(std::move(alarm_rules));

        return catalog;
    }

    std::string_view to_string(
        ApiReadResourceType resource_type
    ) noexcept
    {
        switch (resource_type)
        {
        case ApiReadResourceType::SystemHealth:
            return "SystemHealth";
        case ApiReadResourceType::SystemModules:
            return "SystemModules";
        case ApiReadResourceType::Objects:
            return "Objects";
        case ApiReadResourceType::ObjectTree:
            return "ObjectTree";
        case ApiReadResourceType::Devices:
            return "Devices";
        case ApiReadResourceType::Tags:
            return "Tags";
        case ApiReadResourceType::RuntimeValues:
            return "RuntimeValues";
        case ApiReadResourceType::HistorySamples:
            return "HistorySamples";
        case ApiReadResourceType::Events:
            return "Events";
        case ApiReadResourceType::ActiveAlarms:
            return "ActiveAlarms";
        case ApiReadResourceType::AlarmRules:
            return "AlarmRules";
        case ApiReadResourceType::Unknown:
            return "Unknown";
        }

        return "Unknown";
    }

    std::string_view to_string(
        ApiReadEndpointKind kind
    ) noexcept
    {
        switch (kind)
        {
        case ApiReadEndpointKind::Collection:
            return "Collection";
        case ApiReadEndpointKind::SingleResource:
            return "SingleResource";
        case ApiReadEndpointKind::Tree:
            return "Tree";
        case ApiReadEndpointKind::Query:
            return "Query";
        case ApiReadEndpointKind::StreamPreview:
            return "StreamPreview";
        }

        return "Unknown";
    }
}
