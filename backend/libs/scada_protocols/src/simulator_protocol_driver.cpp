#include "scada_protocols/drivers/simulator_protocol_driver.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <string>

namespace dispatcher::protocols::drivers
{
    namespace
    {
        [[nodiscard]] ProtocolTimestamp now()
        {
            return std::chrono::system_clock::now();
        }

        [[nodiscard]] std::string to_lower_copy(
            std::string_view value
        )
        {
            std::string result(value.begin(), value.end());

            std::transform(
                result.begin(),
                result.end(),
                result.begin(),
                [](unsigned char ch)
                {
                    return static_cast<char>(std::tolower(ch));
                }
            );

            return result;
        }

        [[nodiscard]] bool starts_with(
            std::string_view value,
            std::string_view prefix
        )
        {
            return value.size() >= prefix.size() &&
                value.substr(0, prefix.size()) == prefix;
        }

        [[nodiscard]] bool has_simulator_connection(
            const dispatcher::devices::DeviceConnection& connection
        ) noexcept
        {
            return connection.protocol == dispatcher::devices::DeviceProtocol::Simulator;
        }

        [[nodiscard]] std::string_view address_text(
            const dispatcher::tags::TagAddress& address
        ) noexcept
        {
            if (!address.address.empty())
            {
                return address.address;
            }

            return address.protocol_address_json;
        }
    }

    dispatcher::devices::DeviceProtocol SimulatorProtocolDriver::protocol() const noexcept
    {
        return dispatcher::devices::DeviceProtocol::Simulator;
    }

    std::string_view SimulatorProtocolDriver::name() const noexcept
    {
        return "SimulatorProtocolDriver";
    }

    ProtocolCapabilities SimulatorProtocolDriver::capabilities() const
    {
        ProtocolCapabilities result;

        result.protocol = dispatcher::devices::DeviceProtocol::Simulator;
        result.supports_read = true;
        result.supports_write = true;
        result.supports_batch_read = true;
        result.supports_connection_check = true;
        result.supports_address_test = true;
        result.supports_subscriptions = false;
        result.max_batch_size = 1024;
        result.default_timeout_ms = 100;
        result.supported_value_types = {
            "Boolean",
            "Int32",
            "Int64",
            "Float",
            "Double",
            "String",
            "Enum"
        };

        return result;
    }

    ProtocolConnectionCheckResult SimulatorProtocolDriver::check_connection(
        const ProtocolConnectionCheckRequest& request
    )
    {
        ProtocolConnectionCheckResult result;
        result.timestamp = now();

        if (!has_simulator_connection(request.connection))
        {
            result.success = false;
            result.quality = dispatcher::tags::TagQuality::ProtocolError;
            result.message = "Simulator driver requires Simulator device connection.";
            return result;
        }

        result.success = true;
        result.quality = dispatcher::tags::TagQuality::Good;
        result.latency_ms = 0;
        result.message = "Simulator connection is available.";

        return result;
    }

    ProtocolAddressTestResult SimulatorProtocolDriver::test_address(
        const ProtocolAddressTestRequest& request
    )
    {
        ProtocolAddressTestResult result;
        result.timestamp = now();

        if (!has_simulator_connection(request.connection))
        {
            result.success = false;
            result.quality = dispatcher::tags::TagQuality::ProtocolError;
            result.message = "Simulator driver requires Simulator device connection.";
            return result;
        }

        const auto text = address_text(request.address);

        if (text.empty())
        {
            result.success = false;
            result.quality = dispatcher::tags::TagQuality::Bad;
            result.message = "Simulator address is empty.";
            return result;
        }

        result.success = true;
        result.quality = dispatcher::tags::TagQuality::Good;
        result.normalized_address = std::string(text);
        result.message = "Simulator address is valid.";

        return result;
    }

    ProtocolReadResult SimulatorProtocolDriver::read(
        const ProtocolReadRequest& request
    )
    {
        ProtocolReadResult result;
        result.tag_id = request.tag_id;
        result.server_timestamp = now();
        result.source_timestamp = result.server_timestamp;

        if (!has_simulator_connection(request.connection))
        {
            result.success = false;
            result.quality = dispatcher::tags::TagQuality::ProtocolError;
            result.message = "Simulator driver requires Simulator device connection.";
            return result;
        }

        const auto text = address_text(request.address);

        if (text.empty())
        {
            result.success = false;
            result.quality = dispatcher::tags::TagQuality::Bad;
            result.message = "Simulator address is empty.";
            return result;
        }

        const auto sequence = ++read_counter_;

        result.raw_value = make_value_for_address(text, sequence);
        result.success = true;
        result.quality = dispatcher::tags::TagQuality::Good;
        result.latency_ms = 0;
        result.message = "Simulator read completed.";

        return result;
    }

    ProtocolBatchReadResult SimulatorProtocolDriver::read_batch(
        const ProtocolBatchReadRequest& request
    )
    {
        ProtocolBatchReadResult result;
        result.server_timestamp = now();

        if (!has_simulator_connection(request.connection))
        {
            result.success = false;
            result.message = "Simulator driver requires Simulator device connection.";
            return result;
        }

        if (request.empty())
        {
            result.success = true;
            result.message = "Simulator batch read is empty.";
            return result;
        }

        result.items.reserve(request.items.size());

        bool all_success = true;

        for (const auto& item : request.items)
        {
            auto item_result = read(item);

            if (!item_result.is_success())
            {
                all_success = false;
            }

            result.items.push_back(item_result);
        }

        result.success = all_success;
        result.message = all_success
            ? "Simulator batch read completed."
            : "Simulator batch read completed with item errors.";

        return result;
    }

    ProtocolWriteResult SimulatorProtocolDriver::write(
        const ProtocolWriteRequest& request
    )
    {
        ProtocolWriteResult result;
        result.tag_id = request.tag_id;
        result.dry_run = request.dry_run;
        result.timestamp = now();

        if (!has_simulator_connection(request.connection))
        {
            result.success = false;
            result.quality = dispatcher::tags::TagQuality::ProtocolError;
            result.message = "Simulator driver requires Simulator device connection.";
            return result;
        }

        const auto text = address_text(request.address);

        if (text.empty())
        {
            result.success = false;
            result.quality = dispatcher::tags::TagQuality::Bad;
            result.message = "Simulator address is empty.";
            return result;
        }

        result.success = true;
        result.quality = dispatcher::tags::TagQuality::Good;
        result.latency_ms = 0;

        if (request.dry_run)
        {
            result.message = "Simulator write dry run completed.";
        }
        else
        {
            result.message = "Simulator write accepted.";
        }

        return result;
    }

    dispatcher::tags::TagValuePayload SimulatorProtocolDriver::make_value_for_address(
        std::string_view address,
        std::uint64_t sequence
    ) const
    {
        const auto normalized = to_lower_copy(address);

        if (normalized == "constant.true" ||
            normalized == "bool.true" ||
            normalized == "true")
        {
            return true;
        }

        if (normalized == "constant.false" ||
            normalized == "bool.false" ||
            normalized == "false")
        {
            return false;
        }

        if (normalized == "constant.zero" ||
            normalized == "zero")
        {
            return static_cast<std::int32_t>(0);
        }

        if (normalized == "constant.one" ||
            normalized == "one")
        {
            return static_cast<std::int32_t>(1);
        }

        if (normalized == "status" ||
            normalized == "status.ok" ||
            normalized == "string.status")
        {
            return std::string("OK");
        }

        if (starts_with(normalized, "counter"))
        {
            return static_cast<std::int64_t>(sequence);
        }

        if (starts_with(normalized, "temperature"))
        {
            const auto value = 20.0 + static_cast<double>(sequence % 100) / 10.0;
            return value;
        }

        if (starts_with(normalized, "random"))
        {
            const auto value = static_cast<double>((sequence * 37) % 1000) / 10.0;
            return value;
        }

        return static_cast<double>(sequence);
    }
}
