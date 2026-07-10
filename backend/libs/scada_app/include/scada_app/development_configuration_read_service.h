#pragma once

#include "scada_api/configuration_read_service.h"

#include "scada_devices/device.h"
#include "scada_objects/object_node.h"
#include "scada_tags/tag.h"

#include <cstddef>
#include <vector>

namespace dispatcher::app {

class DevelopmentConfigurationReadService final
    : public dispatcher::api::IConfigurationReadService {
public:
    DevelopmentConfigurationReadService();

    [[nodiscard]] dispatcher::api::ReadCollection<
        dispatcher::api::ObjectReadModel
    > read_objects(
        const dispatcher::api::ObjectReadQuery& query
    ) const override;

    [[nodiscard]] dispatcher::api::ObjectTreeReadResult
    read_object_tree(
        const dispatcher::api::ObjectReadQuery& query
    ) const override;

    [[nodiscard]] dispatcher::api::ReadCollection<
        dispatcher::api::DeviceReadModel
    > read_devices(
        const dispatcher::api::DeviceReadQuery& query
    ) const override;

    [[nodiscard]] dispatcher::api::ReadCollection<
        dispatcher::api::TagReadModel
    > read_tags(
        const dispatcher::api::TagReadQuery& query
    ) const override;

    [[nodiscard]] std::size_t object_count() const noexcept;
    [[nodiscard]] std::size_t device_count() const noexcept;
    [[nodiscard]] std::size_t tag_count() const noexcept;

private:
    std::vector<dispatcher::objects::ObjectNode> objects_;
    std::vector<dispatcher::devices::Device> devices_;
    std::vector<dispatcher::tags::Tag> tags_;
};

[[nodiscard]] dispatcher::api::ConfigurationReadServicePtr
create_development_configuration_read_service();

} // namespace dispatcher::app
