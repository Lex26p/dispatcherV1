#include "scada_http/tag_read_endpoint.h"

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
using dispatcher::http::register_tag_read_endpoint;
using dispatcher::http::tags_endpoint_path;

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
    request.path = "/api/tags";
    request.query = std::move(query);
    request.target = request.path;

    if (!request.query.empty()) {
        request.target += "?";
        request.target += request.query;
    }

    request.correlation_id =
        "tag-request-112";

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
        register_tag_read_endpoint(
            dispatcher,
            service
        )
    );

    return dispatcher;
}

[[nodiscard]] const Json::Value* find_tag(
    const Json::Value& tags,
    const std::string_view id
)
{
    for (
        Json::ArrayIndex index = 0;
        index < tags.size();
        ++index
    ) {
        if (
            tags[index]["id"].asString()
            == id
        ) {
            return &tags[index];
        }
    }

    return nullptr;
}

} // namespace

TEST_CASE(
    "Tag endpoint exposes stable path and registers route",
    "[scada_http][tags]"
)
{
    REQUIRE(
        tags_endpoint_path()
        == "/api/tags"
    );

    const auto dispatcher =
        make_dispatcher();

    REQUIRE(dispatcher.route_count() == 1);

    REQUIRE(
        dispatcher.has_route(
            HttpMethod::Get,
            "/api/tags"
        )
    );
}

TEST_CASE(
    "Tags endpoint returns stable collection contract",
    "[scada_http][tags][json]"
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
        == "tag-request-112"
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["tags"].isArray());
    REQUIRE(root["tags"].size() == 5);
    REQUIRE(root["count"].asUInt64() == 5);
    REQUIRE(root["total"].asUInt64() == 5);
    REQUIRE(root["offset"].asUInt64() == 0);
    REQUIRE(root["limit"].asUInt64() == 1000);
    REQUIRE_FALSE(root["hasMore"].asBool());

    const auto& first =
        root["tags"][Json::ArrayIndex{0}];

    REQUIRE(
        first["id"].asString()
        == "tag-fan-running"
    );

    REQUIRE(
        first["objectId"].asString()
        == "room-control"
    );

    REQUIRE(
        first["deviceId"].asString()
        == "device-modbus-1"
    );

    REQUIRE(
        first["type"].asString()
        == "DeviceTag"
    );

    REQUIRE(
        first["valueType"].asString()
        == "Boolean"
    );

    REQUIRE(
        first["archivePolicy"].asString()
        == "OnQualityChange"
    );

    REQUIRE(
        first["code"].asString()
        == "FAN_RUNNING"
    );

    REQUIRE(first["enabled"].asBool());
}

TEST_CASE(
    "Tags endpoint serializes nullable relationships",
    "[scada_http][tags][null]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "id=tag-system-status"
            )
        );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["total"].asUInt64() == 1);
    REQUIRE(root["tags"].size() == 1);

    const auto& tag =
        root["tags"][Json::ArrayIndex{0}];

    REQUIRE(
        tag["id"].asString()
        == "tag-system-status"
    );

    REQUIRE(tag["objectId"].isNull());
    REQUIRE(tag["deviceId"].isNull());

    REQUIRE(
        tag["type"].asString()
        == "SystemTag"
    );

    REQUIRE(
        tag["valueType"].asString()
        == "String"
    );

    REQUIRE(
        tag["archivePolicy"].asString()
        == "Disabled"
    );

    REQUIRE(tag["address"].asString() == "");
    REQUIRE(tag["enabled"].asBool());
}

TEST_CASE(
    "Tags endpoint includes disabled configuration on request",
    "[scada_http][tags][query]"
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

    REQUIRE(root["total"].asUInt64() == 6);
    REQUIRE(root["tags"].size() == 6);

    const auto* disabled =
        find_tag(
            root["tags"],
            "tag-valve-command-disabled"
        );

    REQUIRE(disabled != nullptr);

    REQUIRE(
        (*disabled)["type"].asString()
        == "CommandTag"
    );

    REQUIRE(
        (*disabled)["valueType"].asString()
        == "Boolean"
    );

    REQUIRE_FALSE(
        (*disabled)["enabled"].asBool()
    );
}

TEST_CASE(
    "Tags endpoint filters by id object and device",
    "[scada_http][tags][query]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto by_id =
        parse_json(
            dispatcher.dispatch(
                make_request(
                    "id=tag-temperature"
                )
            ).body
        );

    REQUIRE(by_id["total"].asUInt64() == 1);

    REQUIRE(
        by_id["tags"][Json::ArrayIndex{0}]
            ["id"].asString()
        == "tag-temperature"
    );

    const auto by_object =
        parse_json(
            dispatcher.dispatch(
                make_request(
                    "objectId=room-control"
                )
            ).body
        );

    REQUIRE(by_object["total"].asUInt64() == 2);

    REQUIRE(
        find_tag(
            by_object["tags"],
            "tag-fan-running"
        ) != nullptr
    );

    REQUIRE(
        find_tag(
            by_object["tags"],
            "tag-temperature"
        ) != nullptr
    );

    const auto by_device =
        parse_json(
            dispatcher.dispatch(
                make_request(
                    "deviceId=device-disabled-1"
                )
            ).body
        );

    REQUIRE(by_device["total"].asUInt64() == 1);

    REQUIRE(
        by_device["tags"][Json::ArrayIndex{0}]
            ["id"].asString()
        == "tag-pump-pressure"
    );
}

TEST_CASE(
    "Tags endpoint applies pagination after ordering",
    "[scada_http][tags][pagination]"
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
    REQUIRE(root["total"].asUInt64() == 6);
    REQUIRE(root["offset"].asUInt64() == 1);
    REQUIRE(root["limit"].asUInt64() == 2);
    REQUIRE(root["hasMore"].asBool());

    REQUIRE(
        root["tags"][Json::ArrayIndex{0}]
            ["code"].asString()
        == "OPERATOR_SETPOINT"
    );

    REQUIRE(
        root["tags"][Json::ArrayIndex{1}]
            ["code"].asString()
        == "PUMP_PRESSURE"
    );
}

TEST_CASE(
    "Tags endpoint serializes engineering configuration",
    "[scada_http][tags][engineering]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "id=tag-temperature"
            )
        );

    const auto root =
        parse_json(response.body);

    const auto& tag =
        root["tags"][Json::ArrayIndex{0}];

    REQUIRE(
        tag["engineeringUnit"].asString()
        == "degC"
    );

    REQUIRE(tag["scale"].asDouble() == 0.1);
    REQUIRE(tag["offset"].asDouble() == 0.0);

    REQUIRE(
        tag["address"].asString()
        == "40001"
    );

    REQUIRE(
        tag["archivePolicy"].asString()
        == "OnChange"
    );
}

TEST_CASE(
    "Tags endpoint rejects invalid query values",
    "[scada_http][tags][error]"
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
        == "tag-request-112"
    );

    REQUIRE(
        boolean_error["error"]["details"]["field"].asString()
        == "includeDisabled"
    );

    REQUIRE(
        boolean_error["error"]["details"]["reason"].asString()
        == "invalid_boolean"
    );

    const auto invalid_limit =
        dispatcher.dispatch(
            make_request(
                "limit=0"
            )
        );

    REQUIRE(
        invalid_limit.status
        == HttpStatusCode::BadRequest
    );

    const auto limit_error =
        parse_json(
            invalid_limit.body
        );

    REQUIRE(
        limit_error["error"]["details"]["field"].asString()
        == "limit"
    );

    REQUIRE(
        limit_error["error"]["details"]["reason"].asString()
        == "must_be_positive"
    );
}

TEST_CASE(
    "Tags endpoint rejects unsupported query parameter",
    "[scada_http][tags][error]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "valueType=Double"
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
        == "valueType"
    );

    REQUIRE(
        root["error"]["details"]["reason"].asString()
        == "unsupported_parameter"
    );
}
