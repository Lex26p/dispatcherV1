#pragma once

#include "scada_http/http_types.h"
#include "scada_http/json_value.h"

#include <string_view>

namespace dispatcher::http {

[[nodiscard]] HttpResponse make_api_error_response(
    HttpStatusCode status,
    std::string_view code,
    std::string_view message,
    std::string_view correlation_id
);

[[nodiscard]] HttpResponse make_api_error_response(
    HttpStatusCode status,
    std::string_view code,
    std::string_view message,
    std::string_view correlation_id,
    JsonValue details
);

} // namespace dispatcher::http
