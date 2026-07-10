#pragma once

#include "scada_api/api_read_endpoint.h"
#include "scada_api/configuration_read_models.h"

#include <memory>
#include <string>

namespace dispatcher::api {

struct ObjectReadQuery {
    ApiReadQueryOptions options;
    ApiReadFilter filter;

    std::string parent_id;

    [[nodiscard]] bool has_parent_id() const noexcept
    {
        return !parent_id.empty();
    }
};

struct DeviceReadQuery {
    ApiReadQueryOptions options;
    ApiReadFilter filter;
};

struct TagReadQuery {
    ApiReadQueryOptions options;
    ApiReadFilter filter;
};

class IConfigurationReadService {
public:
    virtual ~IConfigurationReadService() = default;

    [[nodiscard]] virtual ReadCollection<ObjectReadModel>
    read_objects(
        const ObjectReadQuery& query
    ) const = 0;

    [[nodiscard]] virtual ObjectTreeReadResult
    read_object_tree(
        const ObjectReadQuery& query
    ) const = 0;

    [[nodiscard]] virtual ReadCollection<DeviceReadModel>
    read_devices(
        const DeviceReadQuery& query
    ) const = 0;

    [[nodiscard]] virtual ReadCollection<TagReadModel>
    read_tags(
        const TagReadQuery& query
    ) const = 0;
};

using ConfigurationReadServicePtr =
    std::shared_ptr<
        const IConfigurationReadService
    >;

} // namespace dispatcher::api
