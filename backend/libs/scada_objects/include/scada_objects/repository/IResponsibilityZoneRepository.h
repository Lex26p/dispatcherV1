#pragma once

#include "scada_objects/object_ids.h"
#include "scada_objects/responsibility_zone.h"

#include <optional>
#include <vector>

namespace dispatcher::objects::repository
{
    class IResponsibilityZoneRepository
    {
    public:
        virtual ~IResponsibilityZoneRepository() = default;

        [[nodiscard]] virtual std::vector<ResponsibilityZone> get_all() const = 0;

        [[nodiscard]] virtual std::optional<ResponsibilityZone> find_by_id(
            const ResponsibilityZoneId& id
        ) const = 0;

        [[nodiscard]] virtual std::vector<ResponsibilityZone> find_by_object_id(
            const ObjectId& object_id
        ) const = 0;

        virtual void save(
            const ResponsibilityZone& zone
        ) = 0;

        virtual bool remove_by_id(
            const ResponsibilityZoneId& id
        ) = 0;
    };
}
