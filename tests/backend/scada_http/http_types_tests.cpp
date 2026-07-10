#include "scada_http/http_types.h"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>

namespace {

using dispatcher::http::HttpCorsOptions;
using dispatcher::http::HttpEndpoint;
using dispatcher::http::HttpHeader;
using dispatcher::http::HttpMethod;
using dispatcher::http::HttpRequest;
using dispatcher::http::HttpServerOptions;
using dispatcher::http::HttpStatusCode;
using dispatcher::http::http_method_from_string;
using dispatcher::http::make_empty_response;
using dispatcher::http::make_json_response;
using dispatcher::http::make_text_response;
using dispatcher::http::to_string;

} // namespace

TEST_CASE(
    "HTTP methods are parsed case-insensitively",
    "[scada_http][types][method]"
)
{
    REQUIRE(http_method_from_string("GET") == HttpMethod::Get);
    REQUIRE(http_method_from_string("get") == HttpMethod::Get);
    REQUIRE(http_method_from_string("Post") == HttpMethod::Post);
    REQUIRE(http_method_from_string("PUT") == HttpMethod::Put);
    REQUIRE(http_method_from_string("patch") == HttpMethod::Patch);
    REQUIRE(http_method_from_string("Delete") == HttpMethod::Delete);
    REQUIRE(http_method_from_string("OPTIONS") == HttpMethod::Options);
    REQUIRE(http_method_from_string("head") == HttpMethod::Head);
}

TEST_CASE(
    "Unknown HTTP method is rejected",
    "[scada_http][types][method]"
)
{
    REQUIRE(
        http_method_from_string("TRACE")
        == HttpMethod::Unknown
    );

    REQUIRE(
        http_method_from_string("")
        == HttpMethod::Unknown
    );
}

TEST_CASE(
    "HTTP enum values expose stable names",
    "[scada_http][types]"
)
{
    REQUIRE(to_string(HttpMethod::Get) == "GET");
    REQUIRE(to_string(HttpMethod::Post) == "POST");
    REQUIRE(to_string(HttpMethod::Unknown) == "UNKNOWN");

    REQUIRE(to_string(HttpStatusCode::Ok) == "OK");
    REQUIRE(to_string(HttpStatusCode::BadRequest) == "Bad Request");
    REQUIRE(to_string(HttpStatusCode::NotFound) == "Not Found");
    REQUIRE(
        to_string(HttpStatusCode::MethodNotAllowed)
        == "Method Not Allowed"
    );
}

TEST_CASE(
    "HTTP response factories set status content type and body",
    "[scada_http][types][response]"
)
{
    const auto jsonResponse = make_json_response(
        HttpStatusCode::Created,
        R"({"created":true})"
    );

    REQUIRE(jsonResponse.status == HttpStatusCode::Created);
    REQUIRE(jsonResponse.status_code() == 201);
    REQUIRE(
        jsonResponse.content_type
        == "application/json; charset=utf-8"
    );
    REQUIRE(jsonResponse.body == R"({"created":true})");
    REQUIRE(jsonResponse.has_body());
    REQUIRE(jsonResponse.is_success());

    const auto textResponse = make_text_response(
        HttpStatusCode::Ok,
        "Dispatcher"
    );

    REQUIRE(textResponse.status == HttpStatusCode::Ok);
    REQUIRE(
        textResponse.content_type
        == "text/plain; charset=utf-8"
    );
    REQUIRE(textResponse.body == "Dispatcher");
    REQUIRE(textResponse.is_success());

    const auto emptyResponse = make_empty_response(
        HttpStatusCode::NoContent
    );

    REQUIRE(
        emptyResponse.status
        == HttpStatusCode::NoContent
    );
    REQUIRE_FALSE(emptyResponse.has_body());
    REQUIRE(emptyResponse.is_success());
}

TEST_CASE(
    "Default CORS options are valid for development",
    "[scada_http][types][cors]"
)
{
    const HttpCorsOptions options;

    REQUIRE(options.enabled);
    REQUIRE_FALSE(options.allow_any_origin);
    REQUIRE_FALSE(options.allow_credentials);
    REQUIRE(options.max_age_seconds == 600);

    REQUIRE(options.has_allowed_origins());
    REQUIRE(options.has_allowed_methods());
    REQUIRE(options.has_allowed_headers());
    REQUIRE(options.is_valid());

    REQUIRE(
        std::ranges::find(
            options.allowed_origins,
            "http://localhost:5030"
        ) != options.allowed_origins.end()
    );

    REQUIRE(
        std::ranges::find(
            options.allowed_methods,
            "OPTIONS"
        ) != options.allowed_methods.end()
    );

    REQUIRE(
        std::ranges::find(
            options.allowed_headers,
            "X-Correlation-Id"
        ) != options.allowed_headers.end()
    );
}

TEST_CASE(
    "Disabled CORS does not require policy values",
    "[scada_http][types][cors]"
)
{
    HttpCorsOptions options;

    options.enabled = false;
    options.allowed_origins.clear();
    options.allowed_methods.clear();
    options.allowed_headers.clear();

    REQUIRE(options.is_valid());
}

TEST_CASE(
    "Wildcard CORS cannot be combined with credentials",
    "[scada_http][types][cors]"
)
{
    HttpCorsOptions options;

    options.allow_any_origin = true;
    options.allow_credentials = true;

    REQUIRE_FALSE(options.is_valid());
}

TEST_CASE(
    "Enabled CORS requires origins methods and headers",
    "[scada_http][types][cors]"
)
{
    SECTION("origins are required")
    {
        HttpCorsOptions options;

        options.allow_any_origin = false;
        options.allowed_origins.clear();

        REQUIRE_FALSE(options.is_valid());
    }

    SECTION("methods are required")
    {
        HttpCorsOptions options;

        options.allowed_methods.clear();

        REQUIRE_FALSE(options.is_valid());
    }

    SECTION("headers are required")
    {
        HttpCorsOptions options;

        options.allowed_headers.clear();

        REQUIRE_FALSE(options.is_valid());
    }

    SECTION("wildcard origin satisfies origin requirement")
    {
        HttpCorsOptions options;

        options.allowed_origins.clear();
        options.allow_any_origin = true;
        options.allow_credentials = false;

        REQUIRE(options.is_valid());
    }
}

TEST_CASE(
    "HTTP server options validate required values",
    "[scada_http][types][server]"
)
{
    SECTION("defaults are valid")
    {
        const HttpServerOptions options;

        REQUIRE(options.is_valid());
    }

    SECTION("bind address is required")
    {
        HttpServerOptions options;

        options.bind_address.clear();

        REQUIRE_FALSE(options.is_valid());
    }

    SECTION("port must be greater than zero")
    {
        HttpServerOptions options;

        options.port = 0;

        REQUIRE_FALSE(options.is_valid());
    }

    SECTION("worker count must be greater than zero")
    {
        HttpServerOptions options;

        options.worker_threads = 0;

        REQUIRE_FALSE(options.is_valid());
    }

    SECTION("body limit must be greater than zero")
    {
        HttpServerOptions options;

        options.request_body_limit_bytes = 0;

        REQUIRE_FALSE(options.is_valid());
    }

    SECTION("CORS policy must be valid")
    {
        HttpServerOptions options;

        options.cors.allow_any_origin = true;
        options.cors.allow_credentials = true;

        REQUIRE_FALSE(options.is_valid());
    }
}

TEST_CASE(
    "HTTP request requires method target and path",
    "[scada_http][types][request]"
)
{
    HttpRequest request;

    REQUIRE_FALSE(request.is_valid());

    request.method = HttpMethod::Get;
    REQUIRE_FALSE(request.is_valid());

    request.target = "/api/system/health";
    REQUIRE_FALSE(request.is_valid());

    request.path = "/api/system/health";
    REQUIRE(request.is_valid());

    REQUIRE_FALSE(request.has_body());

    request.body = "request-body";
    REQUIRE(request.has_body());
}

TEST_CASE(
    "HTTP endpoint and header validate required fields",
    "[scada_http][types][endpoint]"
)
{
    HttpHeader header;

    REQUIRE_FALSE(header.is_valid());

    header.name = "X-Correlation-Id";
    REQUIRE(header.is_valid());

    HttpEndpoint endpoint;

    REQUIRE_FALSE(endpoint.is_valid());

    endpoint.path = "/api/test";
    REQUIRE_FALSE(endpoint.is_valid());

    endpoint.name = "test_endpoint";
    REQUIRE(endpoint.is_valid());

    endpoint.method = HttpMethod::Unknown;
    REQUIRE_FALSE(endpoint.is_valid());
}
