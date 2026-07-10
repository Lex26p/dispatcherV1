#include "scada_http/system_health_endpoint.h"

#include "scada_common/version.h"

#include <sstream>
#include <string>

namespace dispatcher::http {
namespace {

[[nodiscard]] std::string json_escape(
    const std::string_view value
)
{
    std::ostringstream output;

    for (const auto character : value) {
        switch (character) {
        case '"':
            output << "\\\"";
            break;
        case '\\':
            output << "\\\\";
            break;
        case '\b':
            output << "\\b";
            break;
        case '\f':
            output << "\\f";
            break;
        case '\n':
            output << "\\n";
            break;
        case '\r':
            output << "\\r";
            break;
        case '\t':
            output << "\\t";
            break;
        default:
            output << character;
            break;
        }
    }

    return output.str();
}

[[nodiscard]] std::string make_health_json()
{
    const auto version = dispatcher::common::get_version_info();

    std::ostringstream output;

    output
        << "{"
        << "\"status\":\"ok\","
        << "\"product\":\"" << json_escape(version.product_name) << "\","
        << "\"executable\":\"" << json_escape(version.executable_name) << "\","
        << "\"version\":\"" << json_escape(version.version) << "\","
        << "\"mode\":\"" << json_escape(version.mode) << "\","
        << "\"api\":\"available\","
        << "\"transport\":\"http\","
        << "\"endpoint\":\"" << json_escape(system_health_endpoint_path()) << "\""
        << "}";

    return output.str();
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
        .path = std::string{system_health_endpoint_path()},
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
