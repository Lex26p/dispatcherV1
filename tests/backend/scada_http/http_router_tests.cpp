#include "scada_http/http_router.h"

#include <catch2/catch_test_macros.hpp>

#include <string>
#include <utility>

namespace {

using dispatcher::http::HttpEndpoint;
using dispatcher::http::HttpMethod;
using dispatcher::http::HttpRequest;
using dispatcher::http::HttpResponse;
using dispatcher::http::HttpRouteDispatcher;
using dispatcher::http::HttpRouteHandler;
using dispatcher::http::HttpStatusCode;
using dispatcher::http::make_json_response;
using dispatcher::http::make_text_response;

[[nodiscard]] HttpEndpoint make_endpoint(
    HttpMethod method,
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
    HttpMethod method,
    std::string path
)
{
    HttpRequest request;

    request.method = method;
    request.target = path;
    request.path = std::move(path);

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
    REQUIRE(dispatcher.has_path("/api/test"));
    REQUIRE(dispatcher.routes().size() == 1);
    REQUIRE(dispatcher.routes().front().is_valid());
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
    bool handlerCalled = false;

    REQUIRE(
        dispatcher.add_route(
            make_endpoint(
                HttpMethod::Post,
                "/api/echo",
                "echo_post"
            ),
            [&handlerCalled](
                const HttpRequest& request
            ) {
                handlerCalled = true;

                REQUIRE(request.method == HttpMethod::Post);
                REQUIRE(request.path == "/api/echo");
                REQUIRE(request.target == "/api/echo?source=test");
                REQUIRE(request.query == "source=test");
                REQUIRE(request.body == "payload");

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

    request.target = "/api/echo?source=test";
    request.query = "source=test";
    request.body = "payload";

    const auto response = dispatcher.dispatch(request);

    REQUIRE(handlerCalled);
    REQUIRE(response.status == HttpStatusCode::Accepted);
    REQUIRE(response.body == "payload");
}

TEST_CASE(
    "Invalid request returns bad request response",
    "[scada_http][router][error]"
)
{
    const HttpRouteDispatcher dispatcher;
    const HttpRequest request;

    const auto response = dispatcher.dispatch(request);

    REQUIRE(response.status == HttpStatusCode::BadRequest);
    REQUIRE(response.status_code() == 400);
    REQUIRE(
        response.content_type
        == "application/json; charset=utf-8"
    );
    REQUIRE(
        response.body.find(R"("code":"bad_request")")
        != std::string::npos
    );
}

TEST_CASE(
    "Unknown path returns not found response",
    "[scada_http][router][error]"
)
{
    const HttpRouteDispatcher dispatcher;

    const auto response = dispatcher.dispatch(
        make_request(
            HttpMethod::Get,
            "/api/missing"
        )
    );

    REQUIRE(response.status == HttpStatusCode::NotFound);
    REQUIRE(response.status_code() == 404);
    REQUIRE(
        response.body.find(R"("code":"not_found")")
        != std::string::npos
    );
    REQUIRE(
        response.body.find("/api/missing")
        != std::string::npos
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

    const auto response = dispatcher.dispatch(
        make_request(
            HttpMethod::Delete,
            "/api/test"
        )
    );

    REQUIRE(
        response.status
        == HttpStatusCode::MethodNotAllowed
    );
    REQUIRE(response.status_code() == 405);
    REQUIRE(
        response.body.find(
            R"("code":"method_not_allowed")"
        ) != std::string::npos
    );
    REQUIRE(
        response.body.find("DELETE")
        != std::string::npos
    );
    REQUIRE(
        response.body.find("/api/test")
        != std::string::npos
    );
}
