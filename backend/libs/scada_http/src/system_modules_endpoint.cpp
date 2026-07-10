#include "scada_http/system_modules_endpoint.h"

#include "scada_http/json_value.h"

#include <cstdint>
#include <string>
#include <utility>

namespace dispatcher::http {
namespace {

[[nodiscard]] std::string make_modules_json(
    const std::vector<dispatcher::core::ModuleInfo>& modules
)
{
    auto module_values = JsonValue::array();

    for (const auto& module : modules) {
        auto module_value = JsonValue::object();

        module_value
            .set_string(
                "code",
                module.code
            )
            .set_string(
                "name",
                module.name
            )
            .set_string(
                "description",
                module.description
            )
            .set_string(
                "status",
                dispatcher::core::to_string(
                    module.status
                )
            );

        module_values.append_value(
            std::move(module_value)
        );
    }

    auto document = JsonValue::object();

    document
        .set_value(
            "modules",
            std::move(module_values)
        )
        .set_uint64(
            "count",
            static_cast<std::uint64_t>(
                modules.size()
            )
        );

    return document.serialize();
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
        .path = std::string{
            system_modules_endpoint_path()
        },
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
        [
            modules = std::move(modules)
        ](const HttpRequest& request) {
            (void)request;

            return make_system_modules_response(
                modules
            );
        }
    );
}

} // namespace dispatcher::http
