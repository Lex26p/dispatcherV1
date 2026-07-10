#include "scada_http/system_health_endpoint.h"
#include "scada_http/system_modules_endpoint.h"

#include "scada_core/module_info.h"

#include <catch2/catch_test_macros.hpp>

#include <json/json.h>

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

using dispatcher::core::ModuleInfo;
using dispatcher::core::ModuleStatus;
using dispatcher::http::HttpStatusCode;
using dispatcher::http::make_system_health_response;
using dispatcher::http::make_system_modules_response;
using dispatcher::http::system_health_endpoint_path;
using dispatcher::http::system_modules_endpoint_path;

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

    const auto parsed = Json::parseFromStream(
        builder,
        input,
        &result,
        &errors
    );

    REQUIRE(parsed);

    return result;
}

} // namespace

TEST_CASE(
    "System health response is valid JSON with stable contract",
    "[scada_http][system][json]"
)
{
    const auto response =
        make_system_health_response();

    REQUIRE(
        response.status
        == HttpStatusCode::Ok
    );

    REQUIRE(
        response.content_type
        == "application/json; charset=utf-8"
    );

    const auto root = parse_json(
        response.body
    );

    REQUIRE(root.isObject());
    REQUIRE(root["status"].asString() == "ok");

    REQUIRE(
        root["product"].asString()
        == "Dispatcher"
    );

    REQUIRE_FALSE(
        root["executable"].asString().empty()
    );

    REQUIRE_FALSE(
        root["version"].asString().empty()
    );

    REQUIRE_FALSE(
        root["mode"].asString().empty()
    );

    REQUIRE(
        root["api"].asString()
        == "available"
    );

    REQUIRE(
        root["transport"].asString()
        == "http"
    );

    REQUIRE(
        root["endpoint"].asString()
        == system_health_endpoint_path()
    );
}

TEST_CASE(
    "System modules response serializes empty collection",
    "[scada_http][system][json]"
)
{
    const std::vector<ModuleInfo> modules;

    const auto response =
        make_system_modules_response(modules);

    REQUIRE(
        response.status
        == HttpStatusCode::Ok
    );

    const auto root = parse_json(
        response.body
    );

    REQUIRE(root["modules"].isArray());
    REQUIRE(root["modules"].empty());
    REQUIRE(root["count"].asUInt64() == 0);
}

TEST_CASE(
    "System modules response preserves module values and escaping",
    "[scada_http][system][json]"
)
{
    const std::vector<ModuleInfo> modules{
        ModuleInfo{
            .code = "module\"one",
            .name = "Module\nOne",
            .description = "Path C:\\Dispatcher\\module",
            .status = ModuleStatus::Running
        },
        ModuleInfo{
            .code = "module_two",
            .name = "Module Two",
            .description = "Degraded module",
            .status = ModuleStatus::Degraded
        }
    };

    const auto response =
        make_system_modules_response(modules);

    const auto root = parse_json(
        response.body
    );

    REQUIRE(root["modules"].isArray());
    REQUIRE(root["modules"].size() == 2);
    REQUIRE(root["count"].asUInt64() == 2);

    const auto& first =
        root["modules"][Json::ArrayIndex{0}];

    REQUIRE(
        first["code"].asString()
        == "module\"one"
    );

    REQUIRE(
        first["name"].asString()
        == "Module\nOne"
    );

    REQUIRE(
        first["description"].asString()
        == "Path C:\\Dispatcher\\module"
    );

    REQUIRE(
        first["status"].asString()
        == "Running"
    );

    const auto& second =
        root["modules"][Json::ArrayIndex{1}];

    REQUIRE(
        second["code"].asString()
        == "module_two"
    );

    REQUIRE(
        second["status"].asString()
        == "Degraded"
    );

    REQUIRE(
        system_modules_endpoint_path()
        == "/api/system/modules"
    );
}
