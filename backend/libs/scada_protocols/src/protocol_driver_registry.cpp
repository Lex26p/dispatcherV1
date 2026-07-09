#include "scada_protocols/protocol_driver_registry.h"

#include "scada_protocols/drivers/simulator_protocol_driver.h"

#include <utility>

namespace dispatcher::protocols
{
    bool ProtocolDriverRegistry::register_driver(
        std::shared_ptr<IProtocolDriver> driver
    )
    {
        if (!driver)
        {
            return false;
        }

        const auto protocol = driver->protocol();

        if (drivers_.contains(protocol))
        {
            return false;
        }

        drivers_.emplace(protocol, std::move(driver));
        return true;
    }

    bool ProtocolDriverRegistry::replace_driver(
        std::shared_ptr<IProtocolDriver> driver
    )
    {
        if (!driver)
        {
            return false;
        }

        const auto protocol = driver->protocol();

        drivers_.insert_or_assign(protocol, std::move(driver));
        return true;
    }

    bool ProtocolDriverRegistry::unregister_driver(
        dispatcher::devices::DeviceProtocol protocol
    )
    {
        return drivers_.erase(protocol) > 0;
    }

    bool ProtocolDriverRegistry::has_driver(
        dispatcher::devices::DeviceProtocol protocol
    ) const
    {
        return drivers_.contains(protocol);
    }

    std::shared_ptr<IProtocolDriver> ProtocolDriverRegistry::find_driver(
        dispatcher::devices::DeviceProtocol protocol
    ) const
    {
        const auto it = drivers_.find(protocol);

        if (it == drivers_.end())
        {
            return nullptr;
        }

        return it->second;
    }

    std::vector<dispatcher::devices::DeviceProtocol> ProtocolDriverRegistry::protocols() const
    {
        std::vector<dispatcher::devices::DeviceProtocol> result;
        result.reserve(drivers_.size());

        for (const auto& item : drivers_)
        {
            result.push_back(item.first);
        }

        return result;
    }

    std::vector<std::shared_ptr<IProtocolDriver>> ProtocolDriverRegistry::drivers() const
    {
        std::vector<std::shared_ptr<IProtocolDriver>> result;
        result.reserve(drivers_.size());

        for (const auto& item : drivers_)
        {
            result.push_back(item.second);
        }

        return result;
    }

    std::size_t ProtocolDriverRegistry::size() const noexcept
    {
        return drivers_.size();
    }

    bool ProtocolDriverRegistry::empty() const noexcept
    {
        return drivers_.empty();
    }

    void ProtocolDriverRegistry::clear() noexcept
    {
        drivers_.clear();
    }

    ProtocolDriverRegistry create_default_protocol_driver_registry()
    {
        ProtocolDriverRegistry registry;

        const auto simulator_registered = registry.register_driver(
            std::make_shared<drivers::SimulatorProtocolDriver>()
        );

        if (!simulator_registered)
        {
            return registry;
        }

        return registry;
    }
}