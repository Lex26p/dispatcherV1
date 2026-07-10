#include "scada_http/device_read_endpoint.h"

#include "scada_http/api_error.h"
#include "scada_http/http_query.h"
#include "scada_http/json_value.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace dispatcher::http {
namespace {

using dispatcher::api::DeviceReadModel;
using dispatcher::api::DeviceReadQuery;
using dispatcher::api::ReadCollection;

struct DeviceQueryParseResult {
    DeviceReadQuery query;
    std::optional<HttpQueryError> error;

    [[nodiscard]] bool is_valid() const noexcept
    {
        return !error.has_value();
    }
};

[[nodiscard]] DeviceQueryParseResult
make_device_query_error(
    std::string field,
    std::string reason
)
{
    DeviceQueryParseResult result;

    result.error = HttpQueryError{
        .field = std::move(field),
        .reason = std::move(reason)
    };

    return result;
}

[[nodiscard]] bool is_supported_device_state(
    const std::string_view state
)
{
    static constexpr std::array<
        std::string_view,
        8
    > supported_states{
        "Draft",
        "Commissioning",
        "Active",
        "Maintenance",
        "Disabled",
        "OutOfService",
        "Decommissioned",
        "Simulation"
    };

    return std::ranges::find(
        supported_states,
        state
    ) != supported_states.end();
}

[[nodiscard]] DeviceQueryParseResult
parse_device_query(
    const std::string_view query_string
)
{
    static constexpr std::array<
        std::string_view,
        6
    > allowed_parameters{
        "id",
        "objectId",
        "state",
        "includeDisabled",
        "limit",
        "offset"
    };

    const auto parsed =
        parse_http_query(
            query_string,
            allowed_parameters
        );

    if (!parsed.is_valid()) {
        DeviceQueryParseResult result;
        result.error = parsed.error;
        return result;
    }

    DeviceQueryParseResult result;

    if (
        const auto id =
            parsed.parameters.value("id");
        id.has_value()
    ) {
        if (id->empty()) {
            return make_device_query_error(
                "id",
                "must_not_be_empty"
            );
        }

        result.query.filter.id =
            std::string{*id};
    }

    if (
        const auto object_id =
            parsed.parameters.value(
                "objectId"
            );
        object_id.has_value()
    ) {
        if (object_id->empty()) {
            return make_device_query_error(
                "objectId",
                "must_not_be_empty"
            );
        }

        result.query.filter.object_id =
            std::string{*object_id};
    }

    if (
        const auto state =
            parsed.parameters.value("state");
        state.has_value()
    ) {
        if (state->empty()) {
            return make_device_query_error(
                "state",
                "must_not_be_empty"
            );
        }

        if (!is_supported_device_state(*state)) {
            return make_device_query_error(
                "state",
                "unsupported_value"
            );
        }

        result.query.filter.state =
            std::string{*state};
    }

    if (
        const auto include_disabled =
            parsed.parameters.value(
                "includeDisabled"
            );
        include_disabled.has_value()
    ) {
        bool value = false;

        if (
            !parse_http_query_bool(
                *include_disabled,
                value
            )
        ) {
            return make_device_query_error(
                "includeDisabled",
                "invalid_boolean"
            );
        }

        result.query.options.include_disabled =
            value;
    }

    if (
        const auto limit =
            parsed.parameters.value("limit");
        limit.has_value()
    ) {
        std::uint32_t value = 0;

        if (
            !parse_http_query_uint32(
                *limit,
                value
            )
        ) {
            return make_device_query_error(
                "limit",
                "invalid_integer"
            );
        }

        if (value == 0) {
            return make_device_query_error(
                "limit",
                "must_be_positive"
            );
        }

        if (
            value
            > result.query.options.max_limit
        ) {
            return make_device_query_error(
                "limit",
                "exceeds_maximum"
            );
        }

        result.query.options.limit = value;
    }

    if (
        const auto offset =
            parsed.parameters.value("offset");
        offset.has_value()
    ) {
        std::uint32_t value = 0;

        if (
            !parse_http_query_uint32(
                *offset,
                value
            )
        ) {
            return make_device_query_error(
                "offset",
                "invalid_integer"
            );
        }

        result.query.options.offset = value;
    }

    return result;
}

[[nodiscard]] JsonValue make_device_json(
    const DeviceReadModel& device
)
{
    auto result = JsonValue::object();

    result.set_string(
        "id",
        device.id
    );

    if (device.object_id.has_value()) {
        result.set_string(
            "objectId",
            device.object_id.value()
        );
    } else {
        result.set_null("objectId");
    }

    auto connection = JsonValue::object();

    connection
        .set_string(
            "host",
            device.connection.host
        )
        .set_uint64(
            "port",
            device.connection.port
        )
        .set_string(
            "ipMode",
            device.connection.ip_mode
        )
        .set_string(
            "networkInterface",
            device.connection.network_interface
        );

    result
        .set_string(
            "code",
            device.code
        )
        .set_string(
            "name",
            device.name
        )
        .set_string(
            "description",
            device.description
        )
        .set_string(
            "protocol",
            device.protocol
        )
        .set_string(
            "state",
            device.state
        )
        .set_bool(
            "runtimeEnabled",
            device.runtime_enabled
        )
        .set_value(
            "connection",
            std::move(connection)
        );

    return result;
}

[[nodiscard]] std::string serialize_devices(
    const ReadCollection<DeviceReadModel>& collection
)
{
    auto devices = JsonValue::array();

    for (const auto& device : collection.items) {
        devices.append_value(
            make_device_json(device)
        );
    }

    auto document = JsonValue::object();

    document
        .set_value(
            "devices",
            std::move(devices)
        )
        .set_uint64(
            "count",
            static_cast<std::uint64_t>(
                collection.count()
            )
        )
        .set_uint64(
            "total",
            collection.total
        )
        .set_uint64(
            "offset",
            collection.offset
        )
        .set_uint64(
            "limit",
            collection.limit
        )
        .set_bool(
            "hasMore",
            collection.has_more
        );

    return document.serialize();
}

[[nodiscard]] HttpResponse
make_invalid_query_response(
    const HttpQueryError& error,
    const std::string_view correlation_id
)
{
    auto details = JsonValue::object();

    details
        .set_string(
            "field",
            error.field
        )
        .set_string(
            "reason",
            error.reason
        );

    return make_api_error_response(
        HttpStatusCode::BadRequest,
        "invalid_query",
        "The query string is invalid.",
        correlation_id,
        std::move(details)
    );
}

[[nodiscard]] HttpResponse
handle_devices_request(
    const HttpRequest& request,
    const dispatcher::api::IConfigurationReadService&
        read_service
)
{
    const auto parsed =
        parse_device_query(
            request.query
        );

    if (!parsed.is_valid()) {
        return make_invalid_query_response(
            parsed.error.value(),
            request.correlation_id
        );
    }

    const auto result =
        read_service.read_devices(
            parsed.query
        );

    return make_json_response(
        HttpStatusCode::Ok,
        serialize_devices(result)
    );
}

} // namespace

std::string_view
devices_endpoint_path() noexcept
{
    return "/api/devices";
}

HttpEndpoint make_devices_endpoint()
{
    return HttpEndpoint{
        .method = HttpMethod::Get,
        .path = std::string{
            devices_endpoint_path()
        },
        .name = "Devices",
        .public_endpoint = true
    };
}

bool register_device_read_endpoint(
    HttpRouteDispatcher& route_dispatcher,
    dispatcher::api::ConfigurationReadServicePtr read_service
)
{
    if (read_service == nullptr) {
        return false;
    }

    return route_dispatcher.add_route(
        make_devices_endpoint(),
        [
            read_service
        ](
            const HttpRequest& request
        ) {
            return handle_devices_request(
                request,
                *read_service
            );
        }
    );
}

} // namespace dispatcher::http
