#include "scada_http/drogon_http_server.h"

#include <drogon/drogon.h>

#include <atomic>
#include <functional>
#include <string>
#include <thread>
#include <utility>

namespace dispatcher::http {
namespace {

[[nodiscard]] drogon::HttpMethod to_drogon_method(
    HttpMethod method
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

[[nodiscard]] HttpRequest to_dispatcher_request(
    const drogon::HttpRequestPtr& request
)
{
    HttpRequest result;

    result.method = http_method_from_string(request->methodString());
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
    result.body = std::string{body.data(), body.size()};

    return result;
}

[[nodiscard]] drogon::HttpResponsePtr to_drogon_response(
    const HttpResponse& response
)
{
    auto result = drogon::HttpResponse::newHttpResponse();

    result->setStatusCode(
        static_cast<drogon::HttpStatusCode>(
            response.status_code()
        )
    );

    result->setContentTypeString(response.content_type);
    result->setBody(response.body);
    result->setCloseConnection(true);

    for (const auto& header : response.headers) {
        if (header.is_valid()) {
            result->addHeader(header.name, header.value);
        }
    }

    return result;
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
        : options(std::move(server_options))
        , route_dispatcher(std::move(dispatcher))
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
            .setThreadNum(options.worker_threads)
            .addListener(options.bind_address, options.port);

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

    void register_routes()
    {
        if (routes_registered) {
            return;
        }

        for (const auto& route : route_dispatcher.routes()) {
            if (!route.is_valid()) {
                continue;
            }

            const auto drogon_method = to_drogon_method(route.endpoint.method);

            if (drogon_method == drogon::Invalid) {
                continue;
            }

            drogon::app().registerHandler(
                route.endpoint.path,
                [route](
                    const drogon::HttpRequestPtr& request,
                    std::function<void(const drogon::HttpResponsePtr&)>&& callback
                ) {
                    const auto dispatcher_request = to_dispatcher_request(request);
                    const auto dispatcher_response = route.handler(dispatcher_request);

                    callback(to_drogon_response(dispatcher_response));
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

DrogonHttpServer::DrogonHttpServer(DrogonHttpServer&&) noexcept = default;

DrogonHttpServer& DrogonHttpServer::operator=(DrogonHttpServer&&) noexcept = default;

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

const HttpServerOptions& DrogonHttpServer::options() const noexcept
{
    return impl_->options;
}

} // namespace dispatcher::http
