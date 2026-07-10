#include "scada_http/drogon_http_server.h"

#include "scada_http/api_error.h"
#include "scada_http/correlation_id.h"

#include <drogon/drogon.h>

#include <algorithm>
#include <atomic>
#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace dispatcher::http {
namespace {

[[nodiscard]] drogon::HttpMethod to_drogon_method(
    const HttpMethod method
)
{
    switch (method) {
    case HttpMethod::Get:
        return drogon::Get;

    case HttpMethod::Post:
        return drogon::Post;

    case HttpMethod::Put:
        return drogon::Put;

    case HttpMethod::Patch:
        return drogon::Patch;

    case HttpMethod::Delete:
        return drogon::Delete;

    case HttpMethod::Options:
        return drogon::Options;

    case HttpMethod::Head:
        return drogon::Head;

    case HttpMethod::Unknown:
    default:
        return drogon::Invalid;
    }
}

[[nodiscard]] std::string join_values(
    const std::vector<std::string>& values
)
{
    std::ostringstream output;

    for (
        std::size_t index = 0;
        index < values.size();
        ++index
    ) {
        if (index > 0) {
            output << ", ";
        }

        output << values[index];
    }

    return output.str();
}

[[nodiscard]] bool is_origin_allowed(
    const HttpCorsOptions& cors_options,
    const std::string& origin
)
{
    if (
        !cors_options.enabled
        || origin.empty()
    ) {
        return false;
    }

    if (cors_options.allow_any_origin) {
        return true;
    }

    return std::ranges::find(
        cors_options.allowed_origins,
        origin
    ) != cors_options.allowed_origins.end();
}

void apply_cors_headers(
    const HttpCorsOptions& cors_options,
    const drogon::HttpRequestPtr& request,
    const drogon::HttpResponsePtr& response
)
{
    if (!cors_options.enabled) {
        return;
    }

    const auto origin =
        request->getHeader("Origin");

    if (
        !is_origin_allowed(
            cors_options,
            origin
        )
    ) {
        return;
    }

    if (
        cors_options.allow_any_origin
        && !cors_options.allow_credentials
    ) {
        response->addHeader(
            "Access-Control-Allow-Origin",
            "*"
        );
    } else {
        response->addHeader(
            "Access-Control-Allow-Origin",
            origin
        );

        response->addHeader(
            "Vary",
            "Origin"
        );
    }

    response->addHeader(
        "Access-Control-Allow-Methods",
        join_values(
            cors_options.allowed_methods
        )
    );

    response->addHeader(
        "Access-Control-Allow-Headers",
        join_values(
            cors_options.allowed_headers
        )
    );

    response->addHeader(
        "Access-Control-Max-Age",
        std::to_string(
            cors_options.max_age_seconds
        )
    );

    if (cors_options.allow_credentials) {
        response->addHeader(
            "Access-Control-Allow-Credentials",
            "true"
        );
    }
}

[[nodiscard]] HttpRequest to_dispatcher_request(
    const drogon::HttpRequestPtr& request
)
{
    HttpRequest result;

    result.method = http_method_from_string(
        request->methodString()
    );

    result.path = request->path();
    result.query = request->query();
    result.target = result.path;

    if (!result.query.empty()) {
        result.target += "?";
        result.target += result.query;
    }

    for (const auto& header : request->headers()) {
        result.headers.push_back(
            HttpHeader{
                .name = header.first,
                .value = header.second
            }
        );
    }

    const auto body = request->body();

    result.body = std::string{
        body.data(),
        body.size()
    };

    result.correlation_id =
        resolve_correlation_id(result);

    return result;
}

[[nodiscard]] drogon::HttpResponsePtr to_drogon_response(
    const HttpResponse& response
)
{
    auto result =
        drogon::HttpResponse::newHttpResponse();

    result->setStatusCode(
        static_cast<drogon::HttpStatusCode>(
            response.status_code()
        )
    );

    result->setContentTypeString(
        response.content_type
    );

    result->setBody(
        response.body
    );

    result->setCloseConnection(true);

    for (const auto& header : response.headers) {
        if (header.is_valid()) {
            result->addHeader(
                header.name,
                header.value
            );
        }
    }

    return result;
}

[[nodiscard]] drogon::HttpResponsePtr
make_cors_preflight_response(
    const std::string_view correlation_id
)
{
    auto response =
        drogon::HttpResponse::newHttpResponse();

    response->setStatusCode(
        static_cast<drogon::HttpStatusCode>(
            static_cast<int>(
                HttpStatusCode::NoContent
            )
        )
    );

    response->setBody("");
    response->setCloseConnection(true);

    response->addHeader(
        std::string{
            correlation_id_header_name
        },
        ensure_correlation_id(
            correlation_id
        )
    );

    return response;
}

[[nodiscard]] HttpResponse make_framework_error_response(
    const drogon::HttpStatusCode status,
    const HttpRequest& request
)
{
    const auto correlation_id =
        resolve_correlation_id(request);

    switch (static_cast<int>(status)) {
    case 400:
        return make_bad_request_response(
            correlation_id
        );

    case 401:
        return make_api_error_response(
            HttpStatusCode::Unauthorized,
            "unauthorized",
            "Authentication is required.",
            correlation_id
        );

    case 403:
        return make_api_error_response(
            HttpStatusCode::Forbidden,
            "forbidden",
            "Access to this resource is forbidden.",
            correlation_id
        );

    case 404:
        return make_not_found_response(
            request.path,
            correlation_id
        );

    case 405:
        return make_method_not_allowed_response(
            request.path,
            request.method,
            correlation_id
        );

    case 409:
        return make_api_error_response(
            HttpStatusCode::Conflict,
            "conflict",
            "The request conflicts with the current state.",
            correlation_id
        );

    case 413:
        return make_api_error_response(
            HttpStatusCode::PayloadTooLarge,
            "payload_too_large",
            "The request body is too large.",
            correlation_id
        );

    case 501:
        return make_api_error_response(
            HttpStatusCode::NotImplemented,
            "not_implemented",
            "The requested operation is not implemented.",
            correlation_id
        );

    case 503:
        return make_api_error_response(
            HttpStatusCode::ServiceUnavailable,
            "service_unavailable",
            "The service is temporarily unavailable.",
            correlation_id
        );

    case 500:
    default:
        return make_internal_server_error_response(
            correlation_id
        );
    }
}

} // namespace

struct DrogonHttpServer::Impl {
    HttpServerOptions options;
    HttpRouteDispatcher route_dispatcher;

    std::thread worker;
    std::atomic_bool running = false;

    bool routes_registered = false;

    Impl(
        HttpServerOptions server_options,
        HttpRouteDispatcher dispatcher
    )
        : options(
            std::move(server_options)
        )
        , route_dispatcher(
            std::move(dispatcher)
        )
    {
    }

    ~Impl()
    {
        stop();
    }

    [[nodiscard]] bool start()
    {
        if (running.load()) {
            return false;
        }

        if (!options.is_valid()) {
            return false;
        }

        register_routes();

        auto& app = drogon::app();

        app
            .setThreadNum(
                options.worker_threads
            )
            .addListener(
                options.bind_address,
                options.port
            );

        running.store(true);

        worker = std::thread{
            []() {
                drogon::app().run();
            }
        };

        return true;
    }

    void stop()
    {
        if (!running.exchange(false)) {
            return;
        }

        drogon::app().quit();

        if (worker.joinable()) {
            worker.join();
        }
    }

    [[nodiscard]] bool is_running() const noexcept
    {
        return running.load();
    }

    void register_framework_error_handler()
    {
        const auto cors_options =
            options.cors;

        drogon::app().setCustomErrorHandler(
            [
                cors_options
            ](
                const drogon::HttpStatusCode status,
                const drogon::HttpRequestPtr& request
            ) {
                const auto dispatcher_request =
                    to_dispatcher_request(
                        request
                    );

                const auto dispatcher_response =
                    make_framework_error_response(
                        status,
                        dispatcher_request
                    );

                const auto drogon_response =
                    to_drogon_response(
                        dispatcher_response
                    );

                apply_cors_headers(
                    cors_options,
                    request,
                    drogon_response
                );

                return drogon_response;
            }
        );
    }

    void register_cors_preflight_advice()
    {
        if (!options.cors.enabled) {
            return;
        }

        std::set<std::string> cors_paths;

        for (const auto& route : route_dispatcher.routes()) {
            if (route.is_valid()) {
                cors_paths.insert(
                    route.endpoint.path
                );
            }
        }

        const auto cors_options =
            options.cors;

        drogon::app().registerPreRoutingAdvice(
            [
                cors_options,
                cors_paths = std::move(cors_paths)
            ](
                const drogon::HttpRequestPtr& request,
                drogon::FilterCallback&& stop,
                drogon::FilterChainCallback&& pass
            ) {
                const auto requested_method =
                    request->getHeader(
                        "Access-Control-Request-Method"
                    );

                const auto is_preflight =
                    request->method() == drogon::Options
                    && !requested_method.empty();

                const auto is_registered_path =
                    cors_paths.find(
                        request->path()
                    ) != cors_paths.end();

                if (
                    !is_preflight
                    || !is_registered_path
                ) {
                    pass();
                    return;
                }

                const auto dispatcher_request =
                    to_dispatcher_request(
                        request
                    );

                auto response =
                    make_cors_preflight_response(
                        dispatcher_request.correlation_id
                    );

                apply_cors_headers(
                    cors_options,
                    request,
                    response
                );

                stop(response);
            }
        );
    }

    void register_routes()
    {
        if (routes_registered) {
            return;
        }

        register_framework_error_handler();
        register_cors_preflight_advice();

        for (const auto& route : route_dispatcher.routes()) {
            if (!route.is_valid()) {
                continue;
            }

            const auto drogon_method =
                to_drogon_method(
                    route.endpoint.method
                );

            if (drogon_method == drogon::Invalid) {
                continue;
            }

            drogon::app().registerHandler(
                route.endpoint.path,
                [this](
                    const drogon::HttpRequestPtr& request,
                    std::function<void(
                        const drogon::HttpResponsePtr&
                    )>&& callback
                ) {
                    const auto dispatcher_request =
                        to_dispatcher_request(
                            request
                        );

                    const auto dispatcher_response =
                        route_dispatcher.dispatch(
                            dispatcher_request
                        );

                    const auto drogon_response =
                        to_drogon_response(
                            dispatcher_response
                        );

                    apply_cors_headers(
                        options.cors,
                        request,
                        drogon_response
                    );

                    callback(
                        drogon_response
                    );
                },
                {drogon_method}
            );
        }

        routes_registered = true;
    }
};

DrogonHttpServer::DrogonHttpServer(
    HttpServerOptions options,
    HttpRouteDispatcher route_dispatcher
)
    : impl_(
        std::make_unique<Impl>(
            std::move(options),
            std::move(route_dispatcher)
        )
    )
{
}

DrogonHttpServer::~DrogonHttpServer() = default;

DrogonHttpServer::DrogonHttpServer(
    DrogonHttpServer&&
) noexcept = default;

DrogonHttpServer& DrogonHttpServer::operator=(
    DrogonHttpServer&&
) noexcept = default;

bool DrogonHttpServer::start()
{
    return impl_->start();
}

void DrogonHttpServer::stop()
{
    impl_->stop();
}

bool DrogonHttpServer::is_running() const noexcept
{
    return impl_->is_running();
}

const HttpServerOptions&
DrogonHttpServer::options() const noexcept
{
    return impl_->options;
}

} // namespace dispatcher::http
