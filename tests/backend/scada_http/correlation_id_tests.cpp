#include "scada_http/correlation_id.h"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <string>

namespace {

using dispatcher::http::HttpHeader;
using dispatcher::http::HttpRequest;
using dispatcher::http::HttpResponse;
using dispatcher::http::correlation_id_header_name;
using dispatcher::http::ensure_correlation_id;
using dispatcher::http::is_valid_correlation_id;
using dispatcher::http::make_correlation_id;
using dispatcher::http::resolve_correlation_id;
using dispatcher::http::set_correlation_id_header;

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
    "Correlation ID validation accepts safe values",
    "[scada_http][correlation]"
)
{
    REQUIRE(
        is_valid_correlation_id(
            "request-123"
        )
    );

    REQUIRE(
        is_valid_correlation_id(
            "client.request_123"
        )
    );

    REQUIRE_FALSE(
        is_valid_correlation_id("")
    );

    REQUIRE_FALSE(
        is_valid_correlation_id(
            "invalid correlation"
        )
    );

    REQUIRE_FALSE(
        is_valid_correlation_id(
            "invalid/value"
        )
    );

    REQUIRE_FALSE(
        is_valid_correlation_id(
            std::string(65, 'a')
        )
    );
}

TEST_CASE(
    "Generated correlation ID is valid",
    "[scada_http][correlation]"
)
{
    const auto first =
        make_correlation_id();

    const auto second =
        make_correlation_id();

    REQUIRE(first.size() == 32);
    REQUIRE(second.size() == 32);

    REQUIRE(
        is_valid_correlation_id(first)
    );

    REQUIRE(
        is_valid_correlation_id(second)
    );

    REQUIRE(first != second);
}

TEST_CASE(
    "Request correlation ID preserves valid incoming header",
    "[scada_http][correlation]"
)
{
    HttpRequest request;

    request.headers.push_back(
        HttpHeader{
            .name = "x-correlation-id",
            .value = "incoming-request-104"
        }
    );

    REQUIRE(
        resolve_correlation_id(request)
        == "incoming-request-104"
    );

    request.correlation_id =
        "normalized-request-104";

    REQUIRE(
        resolve_correlation_id(request)
        == "normalized-request-104"
    );
}

TEST_CASE(
    "Invalid request correlation ID is replaced",
    "[scada_http][correlation]"
)
{
    HttpRequest request;

    request.headers.push_back(
        HttpHeader{
            .name = "X-Correlation-Id",
            .value = "invalid correlation"
        }
    );

    const auto resolved =
        resolve_correlation_id(request);

    REQUIRE(
        is_valid_correlation_id(resolved)
    );

    REQUIRE(
        resolved != "invalid correlation"
    );

    REQUIRE(
        ensure_correlation_id(
            "valid-id"
        ) == "valid-id"
    );
}

TEST_CASE(
    "Response correlation header is added or replaced",
    "[scada_http][correlation]"
)
{
    HttpResponse response;

    set_correlation_id_header(
        response,
        "first-id"
    );

    REQUIRE(
        find_correlation_header(response)
        == "first-id"
    );

    set_correlation_id_header(
        response,
        "second-id"
    );

    REQUIRE(
        find_correlation_header(response)
        == "second-id"
    );

    REQUIRE(
        std::ranges::count_if(
            response.headers,
            [](const HttpHeader& header) {
                return header.name
                    == correlation_id_header_name;
            }
        ) == 1
    );
}
