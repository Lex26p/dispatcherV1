#pragma once

#include "scada_http/http_router.h"
#include "scada_http/http_types.h"

#include <memory>

namespace dispatcher::http {

class DrogonHttpServer final {
public:
    DrogonHttpServer(
        HttpServerOptions options,
        HttpRouteDispatcher route_dispatcher
    );

    ~DrogonHttpServer();

    DrogonHttpServer(const DrogonHttpServer&) = delete;
    DrogonHttpServer& operator=(const DrogonHttpServer&) = delete;

    DrogonHttpServer(DrogonHttpServer&&) noexcept;
    DrogonHttpServer& operator=(DrogonHttpServer&&) noexcept;

    [[nodiscard]] bool start();

    void stop();

    [[nodiscard]] bool is_running() const noexcept;

    [[nodiscard]] const HttpServerOptions& options() const noexcept;

private:
    struct Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace dispatcher::http
