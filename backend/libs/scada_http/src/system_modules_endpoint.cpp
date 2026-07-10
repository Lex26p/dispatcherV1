#include "scada_http/system_modules_endpoint.h"

#include <sstream>
#include <string>
#include <utility>

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

[[nodiscard]] std::string make_modules_json(
    const std::vector<dispatcher::core::ModuleInfo>& modules
)
{
    std::ostringstream output;

    output
        << "{"
        << "\"modules\":[";

    for (std::size_t index = 0; index < modules.size(); ++index) {
        const auto& module = modules[index];

        if (index > 0) {
            output << ",";
        }

        output
            << "{"
            << "\"code\":\"" << json_escape(module.code) << "\","
            << "\"name\":\"" << json_escape(module.name) << "\","
            << "\"description\":\"" << json_escape(module.description) << "\","
            << "\"status\":\"" << json_escape(dispatcher::core::to_string(module.status)) << "\""
            << "}";
    }

    output
        << "],"
        << "\"count\":" << modules.size()
        << "}";

    return output.str();
}

} // namespace

std::string_view system_modules_endpoint_path() noexcept
{
    return "/api/system/modules";
}

HttpEndpoint make_system_modules_endpoint()
{
    return HttpEndpoint{
        .method = HttpMethod::Get,
        .path = std::string{system_modules_endpoint_path()},
        .name = "System modules",
        .public_endpoint = true
    };
}

HttpResponse make_system_modules_response(
    const std::vector<dispatcher::core::ModuleInfo>& modules
)
{
    return make_json_response(
        HttpStatusCode::Ok,
        make_modules_json(modules)
    );
}

bool register_system_modules_endpoint(
    HttpRouteDispatcher& route_dispatcher,
    std::vector<dispatcher::core::ModuleInfo> modules
)
{
    return route_dispatcher.add_route(
        make_system_modules_endpoint(),
        [modules = std::move(modules)](const HttpRequest& request) {
            (void)request;

            return make_system_modules_response(modules);
        }
    );
}

} // namespace dispatcher::http
