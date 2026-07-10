#include "scada_http/device_read_endpoint.h"

#include "scada_app/development_configuration_read_service.h"

#include "scada_http/correlation_id.h"

#include <catch2/catch_test_macros.hpp>

#include <json/json.h>

#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace {

using dispatcher::http::HttpMethod;
using dispatcher::http::HttpRequest;
using dispatcher::http::HttpResponse;
using dispatcher::http::HttpRouteDispatcher;
using dispatcher::http::HttpStatusCode;
using dispatcher::http::correlation_id_header_name;
using dispatcher::http::devices_endpoint_path;
using dispatcher::http::register_device_read_endpoint;

[[nodiscard]] Json::Value parse_json(
    const std::string_view json
)
{
    Json::CharReaderBuilder builder;
    Json::Value result;
    std::string errors;

    std::istringstream input{
        std::string{json}
    };

    const auto parsed =
        Json::parseFromStream(
            builder,
            input,
            &result,
            &errors
        );

    REQUIRE(parsed);

    return result;
}

[[nodiscard]] HttpRequest make_request(
    std::string query = {}
)
{
    HttpRequest request;

    request.method = HttpMethod::Get;
    request.path = "/api/devices";
    request.query = std::move(query);
    request.target = request.path;

    if (!request.query.empty()) {
        request.target += "?";
        request.target += request.query;
    }

    request.correlation_id =
        "device-request-111";

    return request;
}

[[nodiscard]] std::string find_correlation_header(
    const HttpResponse& response
)
{
    for (const auto& header : response.headers) {
        if (
            header.name
            == correlation_id_header_name
        ) {
            return header.value;
        }
    }

    return {};
}

[[nodiscard]] HttpRouteDispatcher
make_dispatcher()
{
    HttpRouteDispatcher dispatcher;

    const auto service =
        dispatcher::app::
            create_development_configuration_read_service();

    REQUIRE(
        register_device_read_endpoint(
            dispatcher,
            service
        )
    );

    return dispatcher;
}

[[nodiscard]] const Json::Value* find_device(
    const Json::Value& devices,
    const std::string_view id
)
{
    for (
        Json::ArrayIndex index = 0;
        index < devices.size();
        ++index
    ) {
        if (
            devices[index]["id"].asString()
            == id
        ) {
            return &devices[index];
        }
    }

    return nullptr;
}

} // namespace

TEST_CASE(
    "Device endpoint exposes stable path and registers route",
    "[scada_http][devices]"
)
{
    REQUIRE(
        devices_endpoint_path()
        == "/api/devices"
    );

    const auto dispatcher =
        make_dispatcher();

    REQUIRE(dispatcher.route_count() == 1);

    REQUIRE(
        dispatcher.has_route(
            HttpMethod::Get,
            "/api/devices"
        )
    );
}

TEST_CASE(
    "Devices endpoint returns stable collection contract",
    "[scada_http][devices][json]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request()
        );

    REQUIRE(
        response.status
        == HttpStatusCode::Ok
    );

    REQUIRE(
        find_correlation_header(response)
        == "device-request-111"
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["devices"].isArray());
    REQUIRE(root["devices"].size() == 3);
    REQUIRE(root["count"].asUInt64() == 3);
    REQUIRE(root["total"].asUInt64() == 3);
    REQUIRE(root["offset"].asUInt64() == 0);
    REQUIRE(root["limit"].asUInt64() == 1000);
    REQUIRE_FALSE(root["hasMore"].asBool());

    const auto& first =
        root["devices"][Json::ArrayIndex{0}];

    REQUIRE(first["id"].asString() == "device-modbus-1");
    REQUIRE(first["objectId"].asString() == "room-control");
    REQUIRE(first["code"].asString() == "PLC_ROOM");
    REQUIRE(first["protocol"].asString() == "ModbusTcp");
    REQUIRE(first["state"].asString() == "Active");
    REQUIRE(first["runtimeEnabled"].asBool());

    REQUIRE(
        first["connection"]["host"].asString()
        == "127.0.0.1"
    );

    REQUIRE(
        first["connection"]["port"].asUInt64()
        == 1502
    );

    REQUIRE(
        first["connection"]["ipMode"].asString()
        == "IPv4"
    );

    REQUIRE(
        first["connection"]["networkInterface"].asString()
        == ""
    );
}

TEST_CASE(
    "Devices endpoint includes disabled configuration on request",
    "[scada_http][devices][query]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "includeDisabled=true"
            )
        );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["total"].asUInt64() == 4);
    REQUIRE(root["devices"].size() == 4);

    const auto* disabled =
        find_device(
            root["devices"],
            "device-disabled-1"
        );

    REQUIRE(disabled != nullptr);

    REQUIRE(
        (*disabled)["state"].asString()
        == "Disabled"
    );

    REQUIRE_FALSE(
        (*disabled)["runtimeEnabled"].asBool()
    );
}

TEST_CASE(
    "Devices endpoint filters by id object and state",
    "[scada_http][devices][query]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto by_id =
        parse_json(
            dispatcher.dispatch(
                make_request(
                    "id=device-snmp-1"
                )
            ).body
        );

    REQUIRE(by_id["total"].asUInt64() == 1);

    REQUIRE(
        by_id["devices"][Json::ArrayIndex{0}]
            ["id"].asString()
        == "device-snmp-1"
    );

    const auto by_object =
        parse_json(
            dispatcher.dispatch(
                make_request(
                    "objectId=room-control"
                )
            ).body
        );

    REQUIRE(by_object["total"].asUInt64() == 1);

    REQUIRE(
        by_object["devices"][Json::ArrayIndex{0}]
            ["id"].asString()
        == "device-modbus-1"
    );

    const auto by_state =
        parse_json(
            dispatcher.dispatch(
                make_request(
                    "state=Maintenance"
                )
            ).body
        );

    REQUIRE(by_state["total"].asUInt64() == 1);

    REQUIRE(
        by_state["devices"][Json::ArrayIndex{0}]
            ["id"].asString()
        == "device-snmp-1"
    );
}

TEST_CASE(
    "Devices endpoint applies pagination after ordering",
    "[scada_http][devices][pagination]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "includeDisabled=true&limit=2&offset=1"
            )
        );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["count"].asUInt64() == 2);
    REQUIRE(root["total"].asUInt64() == 4);
    REQUIRE(root["offset"].asUInt64() == 1);
    REQUIRE(root["limit"].asUInt64() == 2);
    REQUIRE(root["hasMore"].asBool());

    REQUIRE(
        root["devices"][Json::ArrayIndex{0}]
            ["code"].asString()
        == "PUMP_CONTROLLER_DISABLED"
    );

    REQUIRE(
        root["devices"][Json::ArrayIndex{1}]
            ["code"].asString()
        == "SIMULATOR_WATER"
    );
}

TEST_CASE(
    "Devices endpoint serializes IPv6 connection contract",
    "[scada_http][devices][connection]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "id=device-snmp-1"
            )
        );

    const auto root =
        parse_json(response.body);

    const auto& device =
        root["devices"][Json::ArrayIndex{0}];

    REQUIRE(device["objectId"].asString() == "building-admin");
    REQUIRE(device["protocol"].asString() == "Snmp");
    REQUIRE(device["state"].asString() == "Maintenance");

    REQUIRE(
        device["connection"]["host"].asString()
        == "2001:db8::10"
    );

    REQUIRE(
        device["connection"]["port"].asUInt64()
        == 161
    );

    REQUIRE(
        device["connection"]["ipMode"].asString()
        == "IPv6"
    );

    REQUIRE(
        device["connection"]["networkInterface"].asString()
        == "eth0"
    );
}

TEST_CASE(
    "Devices endpoint rejects invalid scalar query",
    "[scada_http][devices][error]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto invalid_boolean =
        dispatcher.dispatch(
            make_request(
                "includeDisabled=yes"
            )
        );

    REQUIRE(
        invalid_boolean.status
        == HttpStatusCode::BadRequest
    );

    const auto boolean_error =
        parse_json(
            invalid_boolean.body
        );

    REQUIRE(
        boolean_error["error"]["code"].asString()
        == "invalid_query"
    );

    REQUIRE(
        boolean_error["error"]["correlationId"].asString()
        == "device-request-111"
    );

    REQUIRE(
        boolean_error["error"]["details"]["field"].asString()
        == "includeDisabled"
    );

    REQUIRE(
        boolean_error["error"]["details"]["reason"].asString()
        == "invalid_boolean"
    );

    const auto invalid_state =
        dispatcher.dispatch(
            make_request(
                "state=UnknownState"
            )
        );

    REQUIRE(
        invalid_state.status
        == HttpStatusCode::BadRequest
    );

    const auto state_error =
        parse_json(
            invalid_state.body
        );

    REQUIRE(
        state_error["error"]["details"]["field"].asString()
        == "state"
    );

    REQUIRE(
        state_error["error"]["details"]["reason"].asString()
        == "unsupported_value"
    );
}

TEST_CASE(
    "Devices endpoint rejects unsupported query parameter",
    "[scada_http][devices][error]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "protocol=Snmp"
            )
        );

    REQUIRE(
        response.status
        == HttpStatusCode::BadRequest
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(
        root["error"]["details"]["field"].asString()
        == "protocol"
    );

    REQUIRE(
        root["error"]["details"]["reason"].asString()
        == "unsupported_parameter"
    );
}
