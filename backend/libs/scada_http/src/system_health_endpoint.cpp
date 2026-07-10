#include "scada_http/system_health_endpoint.h"

#include "scada_common/version.h"

#include "scada_http/json_value.h"

#include <string>

namespace dispatcher::http {
namespace {

[[nodiscard]] std::string make_health_json()
{
    const auto version =
        dispatcher::common::get_version_info();

    auto document = JsonValue::object();

    document
        .set_string("status", "ok")
        .set_string(
            "product",
            version.product_name
        )
        .set_string(
            "executable",
            version.executable_name
        )
        .set_string(
            "version",
            version.version
        )
        .set_string(
            "mode",
            version.mode
        )
        .set_string(
            "api",
            "available"
        )
        .set_string(
            "transport",
            "http"
        )
        .set_string(
            "endpoint",
            system_health_endpoint_path()
        );

    return document.serialize();
}

} // namespace

std::string_view system_health_endpoint_path() noexcept
{
    return "/api/system/health";
}

HttpEndpoint make_system_health_endpoint()
{
    return HttpEndpoint{
        .method = HttpMethod::Get,
        .path = std::string{
            system_health_endpoint_path()
        },
        .name = "System health",
        .public_endpoint = true
    };
}

HttpResponse make_system_health_response()
{
    return make_json_response(
        HttpStatusCode::Ok,
        make_health_json()
    );
}

bool register_system_health_endpoint(
    HttpRouteDispatcher& route_dispatcher
)
{
    return route_dispatcher.add_route(
        make_system_health_endpoint(),
        [](const HttpRequest& request) {
            (void)request;

            return make_system_health_response();
        }
    );
}

} // namespace dispatcher::http
