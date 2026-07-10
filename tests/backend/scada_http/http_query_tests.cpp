#include "scada_http/http_query.h"

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cstdint>
#include <string_view>

namespace {

using dispatcher::http::parse_http_query;
using dispatcher::http::parse_http_query_bool;
using dispatcher::http::parse_http_query_uint32;

constexpr std::array<
    std::string_view,
    4
> allowed_parameters{
    "id",
    "name",
    "enabled",
    "limit"
};

} // namespace

TEST_CASE(
    "Empty HTTP query is valid",
    "[scada_http][query]"
)
{
    const auto result =
        parse_http_query(
            "",
            allowed_parameters
        );

    REQUIRE(result.is_valid());
    REQUIRE(result.parameters.empty());
}

TEST_CASE(
    "HTTP query decodes names and values",
    "[scada_http][query]"
)
{
    const auto result =
        parse_http_query(
            "?id=object%2D1&name=Control+Room",
            allowed_parameters
        );

    REQUIRE(result.is_valid());
    REQUIRE(result.parameters.size() == 2);

    REQUIRE(
        result.parameters.value("id")
        == "object-1"
    );

    REQUIRE(
        result.parameters.value("name")
        == "Control Room"
    );
}

TEST_CASE(
    "HTTP query rejects unsupported parameter",
    "[scada_http][query]"
)
{
    const auto result =
        parse_http_query(
            "unexpected=value",
            allowed_parameters
        );

    REQUIRE_FALSE(result.is_valid());
    REQUIRE(result.error.has_value());

    REQUIRE(
        result.error->field
        == "unexpected"
    );

    REQUIRE(
        result.error->reason
        == "unsupported_parameter"
    );
}

TEST_CASE(
    "HTTP query rejects duplicate parameter",
    "[scada_http][query]"
)
{
    const auto result =
        parse_http_query(
            "id=first&id=second",
            allowed_parameters
        );

    REQUIRE_FALSE(result.is_valid());

    REQUIRE(
        result.error->field
        == "id"
    );

    REQUIRE(
        result.error->reason
        == "duplicate_parameter"
    );
}

TEST_CASE(
    "HTTP query rejects malformed percent encoding",
    "[scada_http][query]"
)
{
    const auto result =
        parse_http_query(
            "id=object%2",
            allowed_parameters
        );

    REQUIRE_FALSE(result.is_valid());

    REQUIRE(
        result.error->reason
        == "invalid_encoding"
    );
}

TEST_CASE(
    "HTTP query scalar parsers validate complete values",
    "[scada_http][query]"
)
{
    bool boolean_value = false;

    REQUIRE(
        parse_http_query_bool(
            "true",
            boolean_value
        )
    );

    REQUIRE(boolean_value);

    REQUIRE(
        parse_http_query_bool(
            "0",
            boolean_value
        )
    );

    REQUIRE_FALSE(boolean_value);

    REQUIRE_FALSE(
        parse_http_query_bool(
            "yes",
            boolean_value
        )
    );

    std::uint32_t integer_value = 0;

    REQUIRE(
        parse_http_query_uint32(
            "123",
            integer_value
        )
    );

    REQUIRE(integer_value == 123);

    REQUIRE_FALSE(
        parse_http_query_uint32(
            "-1",
            integer_value
        )
    );

    REQUIRE_FALSE(
        parse_http_query_uint32(
            "12x",
            integer_value
        )
    );
}
