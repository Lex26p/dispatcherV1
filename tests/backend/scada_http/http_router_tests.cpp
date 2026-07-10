#include "scada_http/http_router.h"

#include "scada_http/correlation_id.h"

#include <catch2/catch_test_macros.hpp>

#include <json/json.h>

#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {

using dispatcher::http::HttpEndpoint;
using dispatcher::http::HttpHeader;
using dispatcher::http::HttpMethod;
using dispatcher::http::HttpRequest;
using dispatcher::http::HttpResponse;
using dispatcher::http::HttpRouteDispatcher;
using dispatcher::http::HttpRouteHandler;
using dispatcher::http::HttpStatusCode;
using dispatcher::http::correlation_id_header_name;
using dispatcher::http::make_json_response;
using dispatcher::http::make_text_response;

[[nodiscard]] HttpEndpoint make_endpoint(
    const HttpMethod method,
    std::string path,
    std::string name
)
{
    return HttpEndpoint{
        .method = method,
        .path = std::move(path),
        .name = std::move(name),
        .public_endpoint = true
    };
}

[[nodiscard]] HttpRequest make_request(
    const HttpMethod method,
    std::string path
)
{
    HttpRequest request;

    request.method = method;
    request.target = path;
    request.path = std::move(path);

    request.correlation_id =
        "router-request-104";

    return request;
}

[[nodiscard]] HttpRouteHandler make_ok_handler()
{
    return [](
        const HttpRequest&
    ) {
        return make_json_response(
            HttpStatusCode::Ok,
            R"({"status":"ok"})"
        );
    };
}

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
    "Valid route can be added and discovered",
    "[scada_http][router]"
)
{
    HttpRouteDispatcher dispatcher;

    REQUIRE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Get,
                "/api/test",
                "test_get"
            ),
            make_ok_handler()
        )
    );

    REQUIRE(dispatcher.route_count() == 1);

    REQUIRE(
        dispatcher.has_route(
            HttpMethod::Get,
            "/api/test"
        )
    );

    REQUIRE(
        dispatcher.has_path(
            "/api/test"
        )
    );

    REQUIRE(
        dispatcher.routes().size() == 1
    );

    REQUIRE(
        dispatcher.routes().front().is_valid()
    );
}

TEST_CASE(
    "Duplicate method and path are rejected",
    "[scada_http][router]"
)
{
    HttpRouteDispatcher dispatcher;

    REQUIRE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Get,
                "/api/test",
                "test_get"
            ),
            make_ok_handler()
        )
    );

    REQUIRE_FALSE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Get,
                "/api/test",
                "duplicate_test_get"
            ),
            make_ok_handler()
        )
    );

    REQUIRE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Post,
                "/api/test",
                "test_post"
            ),
            make_ok_handler()
        )
    );

    REQUIRE(dispatcher.route_count() == 2);
}

TEST_CASE(
    "Invalid endpoint or empty handler is rejected",
    "[scada_http][router]"
)
{
    HttpRouteDispatcher dispatcher;

    REQUIRE_FALSE(
        dispatcher.add_route(
            HttpEndpoint{},
            make_ok_handler()
        )
    );

    REQUIRE_FALSE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Get,
                "/api/test",
                "test_get"
            ),
            HttpRouteHandler{}
        )
    );

    REQUIRE(dispatcher.route_count() == 0);
}

TEST_CASE(
    "Dispatcher invokes matching route handler",
    "[scada_http][router]"
)
{
    HttpRouteDispatcher dispatcher;
    bool handler_called = false;

    REQUIRE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Post,
                "/api/echo",
                "echo_post"
            ),
            [
                &handler_called
            ](
                const HttpRequest& request
            ) {
                handler_called = true;

                REQUIRE(
                    request.method
                    == HttpMethod::Post
                );

                REQUIRE(
                    request.path
                    == "/api/echo"
                );

                REQUIRE(
                    request.target
                    == "/api/echo?source=test"
                );

                REQUIRE(
                    request.query
                    == "source=test"
                );

                REQUIRE(
                    request.body
                    == "payload"
                );

                REQUIRE(
                    request.correlation_id
                    == "router-request-104"
                );

                return make_text_response(
                    HttpStatusCode::Accepted,
                    request.body
                );
            }
        )
    );

    auto request = make_request(
        HttpMethod::Post,
        "/api/echo"
    );

    request.target =
        "/api/echo?source=test";

    request.query =
        "source=test";

    request.body =
        "payload";

    const auto response =
        dispatcher.dispatch(request);

    REQUIRE(handler_called);

    REQUIRE(
        response.status
        == HttpStatusCode::Accepted
    );

    REQUIRE(
        response.body
        == "payload"
    );

    REQUIRE(
        find_correlation_header(response)
        == "router-request-104"
    );
}

TEST_CASE(
    "Invalid request returns correlated bad request",
    "[scada_http][router][error]"
)
{
    const HttpRouteDispatcher dispatcher;

    HttpRequest request;

    request.correlation_id =
        "bad-request-104";

    const auto response =
        dispatcher.dispatch(request);

    REQUIRE(
        response.status
        == HttpStatusCode::BadRequest
    );

    REQUIRE(
        response.status_code() == 400
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(
        root["error"]["code"].asString()
        == "bad_request"
    );

    REQUIRE(
        root["error"]["correlationId"].asString()
        == "bad-request-104"
    );
}

TEST_CASE(
    "Unknown path returns correlated not found response",
    "[scada_http][router][error]"
)
{
    const HttpRouteDispatcher dispatcher;

    const auto response =
        dispatcher.dispatch(
            make_request(
                HttpMethod::Get,
                "/api/missing"
            )
        );

    REQUIRE(
        response.status
        == HttpStatusCode::NotFound
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(
        root["error"]["code"].asString()
        == "not_found"
    );

    REQUIRE(
        root["error"]["correlationId"].asString()
        == "router-request-104"
    );

    REQUIRE(
        root["error"]["details"]["path"].asString()
        == "/api/missing"
    );
}

TEST_CASE(
    "Known path with different method returns method not allowed",
    "[scada_http][router][error]"
)
{
    HttpRouteDispatcher dispatcher;

    REQUIRE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Get,
                "/api/test",
                "test_get"
            ),
            make_ok_handler()
        )
    );

    const auto response =
        dispatcher.dispatch(
            make_request(
                HttpMethod::Delete,
                "/api/test"
            )
        );

    REQUIRE(
        response.status
        == HttpStatusCode::MethodNotAllowed
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(
        root["error"]["code"].asString()
        == "method_not_allowed"
    );

    REQUIRE(
        root["error"]["details"]["method"].asString()
        == "DELETE"
    );

    REQUIRE(
        root["error"]["details"]["path"].asString()
        == "/api/test"
    );
}

TEST_CASE(
    "Dispatcher adds generated correlation ID to success response",
    "[scada_http][router][correlation]"
)
{
    HttpRouteDispatcher dispatcher;

    REQUIRE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Get,
                "/api/test",
                "test_get"
            ),
            make_ok_handler()
        )
    );

    auto request =
        make_request(
            HttpMethod::Get,
            "/api/test"
        );

    request.correlation_id.clear();

    const auto response =
        dispatcher.dispatch(request);

    const auto correlation_id =
        find_correlation_header(response);

    REQUIRE_FALSE(
        correlation_id.empty()
    );
}

TEST_CASE(
    "Handler exception returns safe internal server error",
    "[scada_http][router][error]"
)
{
    HttpRouteDispatcher dispatcher;

    REQUIRE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Get,
                "/api/failure",
                "failure_get"
            ),
            [](
                const HttpRequest&
            ) -> HttpResponse {
                throw std::runtime_error{
                    "Sensitive internal information."
                };
            }
        )
    );

    const auto response =
        dispatcher.dispatch(
            make_request(
                HttpMethod::Get,
                "/api/failure"
            )
        );

    REQUIRE(
        response.status
        == HttpStatusCode::InternalServerError
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(
        root["error"]["code"].asString()
        == "internal_server_error"
    );

    REQUIRE(
        root["error"]["message"].asString()
        == "An internal server error occurred."
    );

    REQUIRE(
        response.body.find(
            "Sensitive internal information."
        ) == std::string::npos
    );
}
