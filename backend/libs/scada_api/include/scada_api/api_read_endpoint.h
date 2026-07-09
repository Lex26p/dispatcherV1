#pragma once

#include "scada_api/api_endpoint.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::api
{
    enum class ApiReadResourceType
    {
        SystemHealth,
        SystemModules,
        Objects,
        ObjectTree,
        Devices,
        Tags,
        RuntimeValues,
        HistorySamples,
        Events,
        ActiveAlarms,
        AlarmRules,
        Unknown
    };

    enum class ApiReadEndpointKind
    {
        Collection,
        SingleResource,
        Tree,
        Query,
        StreamPreview
    };

    struct ApiReadQueryOptions
    {
        std::uint32_t limit = 1000;
        std::uint32_t offset = 0;
        std::uint32_t max_limit = 10000;

        bool include_metadata = true;
        bool include_disabled = false;
        bool include_children = false;
        bool include_raw_values = true;

        [[nodiscard]] std::uint32_t normalized_limit() const noexcept;
        [[nodiscard]] bool has_offset() const noexcept;
    };

    struct ApiReadFilter
    {
        std::string id;
        std::string object_id;
        std::string device_id;
        std::string tag_id;
        std::string source_id;
        std::string correlation_id;

        std::string state;
        std::string quality;
        std::string severity;
        std::string priority;

        std::string from_timestamp;
        std::string to_timestamp;

        [[nodiscard]] bool has_id() const noexcept;
        [[nodiscard]] bool has_object_id() const noexcept;
        [[nodiscard]] bool has_device_id() const noexcept;
        [[nodiscard]] bool has_tag_id() const noexcept;
        [[nodiscard]] bool has_source_id() const noexcept;
        [[nodiscard]] bool has_correlation_id() const noexcept;
        [[nodiscard]] bool has_state() const noexcept;
        [[nodiscard]] bool has_quality() const noexcept;
        [[nodiscard]] bool has_severity() const noexcept;
        [[nodiscard]] bool has_priority() const noexcept;
        [[nodiscard]] bool has_time_range() const noexcept;
        [[nodiscard]] bool has_any_filter() const noexcept;
    };

    struct ApiReadEndpointDefinition
    {
        ApiReadResourceType resource_type = ApiReadResourceType::Unknown;
        ApiReadEndpointKind kind = ApiReadEndpointKind::Collection;

        ApiEndpoint endpoint;

        bool supports_filtering = false;
        bool supports_pagination = true;
        bool supports_time_range = false;
        bool supports_realtime = false;
        bool returns_collection = true;

        [[nodiscard]] bool has_endpoint() const noexcept;
        [[nodiscard]] bool is_valid() const noexcept;
    };

    class ApiReadEndpointCatalog
    {
    public:
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;

        [[nodiscard]] const std::vector<ApiReadEndpointDefinition>& definitions() const noexcept;

        bool register_definition(
            const ApiReadEndpointDefinition& definition
        );

        bool register_definition(
            ApiReadEndpointDefinition&& definition
        );

        [[nodiscard]] std::vector<ApiReadEndpointDefinition> find_by_resource_type(
            ApiReadResourceType resource_type
        ) const;

        [[nodiscard]] std::vector<ApiReadEndpointDefinition> find_by_area(
            ApiEndpointArea area
        ) const;

        void clear() noexcept;

    private:
        std::vector<ApiReadEndpointDefinition> definitions_;

        [[nodiscard]] bool contains_path(
            ApiHttpMethod method,
            const std::string& path
        ) const;
    };

    [[nodiscard]] ApiReadEndpointDefinition make_api_read_endpoint_definition(
        ApiReadResourceType resource_type,
        ApiReadEndpointKind kind,
        ApiEndpoint endpoint
    );

    [[nodiscard]] ApiReadEndpointCatalog create_default_api_read_endpoint_catalog();

    [[nodiscard]] std::string_view to_string(
        ApiReadResourceType resource_type
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        ApiReadEndpointKind kind
    ) noexcept;
}