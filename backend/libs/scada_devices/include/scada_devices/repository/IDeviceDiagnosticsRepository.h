#pragma once

#include "scada_devices/device_diagnostics.h"
#include "scada_devices/device_ids.h"

#include <optional>
#include <vector>

namespace dispatcher::devices::repository
{
    class IDeviceDiagnosticsRepository
    {
    public:
        virtual ~IDeviceDiagnosticsRepository() = default;

        [[nodiscard]] virtual std::vector<DeviceDiagnostics> get_all() const = 0;

        [[nodiscard]] virtual std::optional<DeviceDiagnostics> find_by_device_id(
            const DeviceId& device_id
        ) const = 0;

        virtual void save(
            const DeviceDiagnostics& diagnostics
        ) = 0;

        virtual bool remove_by_device_id(
            const DeviceId& device_id
        ) = 0;
    };
}