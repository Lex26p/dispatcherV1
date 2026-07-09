#pragma once

#include "scada_devices/device_protocol.h"
#include "scada_protocols/protocol_driver.h"

#include <cstddef>
#include <map>
#include <memory>
#include <vector>

namespace dispatcher::protocols
{
    class ProtocolDriverRegistry
    {
    public:
        [[nodiscard]] bool register_driver(
            std::shared_ptr<IProtocolDriver> driver
        );

        [[nodiscard]] bool replace_driver(
            std::shared_ptr<IProtocolDriver> driver
        );

        [[nodiscard]] bool unregister_driver(
            dispatcher::devices::DeviceProtocol protocol
        );

        [[nodiscard]] bool has_driver(
            dispatcher::devices::DeviceProtocol protocol
        ) const;

        [[nodiscard]] std::shared_ptr<IProtocolDriver> find_driver(
            dispatcher::devices::DeviceProtocol protocol
        ) const;

        [[nodiscard]] std::vector<dispatcher::devices::DeviceProtocol> protocols() const;

        [[nodiscard]] std::vector<std::shared_ptr<IProtocolDriver>> drivers() const;

        [[nodiscard]] std::size_t size() const noexcept;

        [[nodiscard]] bool empty() const noexcept;

        void clear() noexcept;

    private:
        std::map<
            dispatcher::devices::DeviceProtocol,
            std::shared_ptr<IProtocolDriver>
        > drivers_;
    };

    [[nodiscard]] ProtocolDriverRegistry create_default_protocol_driver_registry();
}