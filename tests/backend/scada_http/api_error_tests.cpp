#include "scada_http/api_error.h"

#include "scada_http/correlation_id.h"

#include <catch2/catch_test_macros.hpp>

#include <json/json.h>

#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace {

using dispatcher::http::HttpResponse;
using dispatcher::http::HttpStatusCode;
using dispatcher::http::JsonValue;
using dispatcher::http::correlation_id_header_name;
using dispatcher::http::make_api_error_response;

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

} // namespace

TEST_CASE(
    "API error response has stable envelope",
    "[scada_http][error]"
)
{
    const auto response =
        make_api_error_response(
            HttpStatusCode::BadRequest,
            "bad_request",
            "Request is invalid.",
            "error-request-104"
        );

    REQUIRE(
        response.status
        == HttpStatusCode::BadRequest
    );

    REQUIRE(
        find_correlation_header(response)
        == "error-request-104"
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["error"].isObject());

    REQUIRE(
        root["error"]["code"].asString()
        == "bad_request"
    );

    REQUIRE(
        root["error"]["message"].asString()
        == "Request is invalid."
    );

    REQUIRE(
        root["error"]["correlationId"].asString()
        == "error-request-104"
    );

    REQUIRE(
        root["error"]["details"].isNull()
    );
}

TEST_CASE(
    "API error response serializes structured details",
    "[scada_http][error]"
)
{
    auto details =
        JsonValue::object();

    details
        .set_string(
            "path",
            "/api/missing"
        )
        .set_string(
            "method",
            "GET"
        );

    const auto response =
        make_api_error_response(
            HttpStatusCode::NotFound,
            "not_found",
            "Route was not found.",
            "details-request-104",
            std::move(details)
        );

    const auto root =
        parse_json(response.body);

    REQUIRE(
        root["error"]["details"]["path"].asString()
        == "/api/missing"
    );

    REQUIRE(
        root["error"]["details"]["method"].asString()
        == "GET"
    );
}
