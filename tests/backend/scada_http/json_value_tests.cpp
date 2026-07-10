#include "scada_http/json_value.h"

#include <catch2/catch_test_macros.hpp>

#include <json/json.h>

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {

using dispatcher::http::JsonValue;

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
    "JSON value serializes scalar object members",
    "[scada_http][json]"
)
{
    auto document = JsonValue::object();

    document
        .set_string(
            "text",
            "quote \" slash \\ line\nnext"
        )
        .set_bool(
            "enabled",
            true
        )
        .set_int64(
            "signed",
            std::int64_t{-42}
        )
        .set_uint64(
            "unsigned",
            std::uint64_t{42}
        )
        .set_double(
            "number",
            3.5
        )
        .set_null(
            "optional"
        );

    const auto root = parse_json(
        document.serialize()
    );

    REQUIRE(root.isObject());

    REQUIRE(
        root["text"].asString()
        == "quote \" slash \\ line\nnext"
    );

    REQUIRE(root["enabled"].asBool());
    REQUIRE(root["signed"].asInt64() == -42);
    REQUIRE(root["unsigned"].asUInt64() == 42);
    REQUIRE(root["number"].asDouble() == 3.5);
    REQUIRE(root["optional"].isNull());
}

TEST_CASE(
    "JSON value serializes nested objects and arrays",
    "[scada_http][json]"
)
{
    auto child = JsonValue::object();

    child
        .set_string("code", "scada_http")
        .set_bool("running", true);

    auto values = JsonValue::array();

    values
        .append_string("first")
        .append_bool(false)
        .append_int64(-7)
        .append_uint64(7)
        .append_double(1.25)
        .append_null()
        .append_value(
            std::move(child)
        );

    auto document = JsonValue::object();

    document.set_value(
        "values",
        std::move(values)
    );

    const auto root = parse_json(
        document.serialize()
    );

    const auto& parsed_values =
        root["values"];

    REQUIRE(parsed_values.isArray());
    REQUIRE(parsed_values.size() == 7);

    REQUIRE(
        parsed_values[Json::ArrayIndex{0}].asString()
        == "first"
    );

    REQUIRE_FALSE(
        parsed_values[Json::ArrayIndex{1}].asBool()
    );

    REQUIRE(
        parsed_values[Json::ArrayIndex{2}].asInt64()
        == -7
    );

    REQUIRE(
        parsed_values[Json::ArrayIndex{3}].asUInt64()
        == 7
    );

    REQUIRE(
        parsed_values[Json::ArrayIndex{4}].asDouble()
        == 1.25
    );

    REQUIRE(
        parsed_values[Json::ArrayIndex{5}].isNull()
    );

    REQUIRE(
        parsed_values[Json::ArrayIndex{6}]["code"].asString()
        == "scada_http"
    );

    REQUIRE(
        parsed_values[Json::ArrayIndex{6}]["running"].asBool()
    );
}

TEST_CASE(
    "JSON null root serializes as valid null",
    "[scada_http][json]"
)
{
    auto document = JsonValue::null();

    REQUIRE(document.is_null());
    REQUIRE_FALSE(document.is_object());
    REQUIRE_FALSE(document.is_array());

    const auto root = parse_json(
        document.serialize()
    );

    REQUIRE(root.isNull());
}

TEST_CASE(
    "JSON value rejects invalid container operations",
    "[scada_http][json]"
)
{
    auto object = JsonValue::object();
    auto array = JsonValue::array();
    auto null_value = JsonValue::null();

    REQUIRE_THROWS_AS(
        object.append_string("invalid"),
        std::logic_error
    );

    REQUIRE_THROWS_AS(
        array.set_string(
            "key",
            "invalid"
        ),
        std::logic_error
    );

    REQUIRE_THROWS_AS(
        null_value.set_null("key"),
        std::logic_error
    );
}
