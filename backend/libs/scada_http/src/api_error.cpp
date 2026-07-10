#include "scada_http/api_error.h"

#include "scada_http/correlation_id.h"

#include <string>
#include <utility>

namespace dispatcher::http {
namespace {

[[nodiscard]] HttpResponse make_error_response(
    const HttpStatusCode status,
    const std::string_view code,
    const std::string_view message,
    const std::string& correlation_id,
    JsonValue details
)
{
    auto error = JsonValue::object();

    error
        .set_string(
            "code",
            code
        )
        .set_string(
            "message",
            message
        )
        .set_string(
            "correlationId",
            correlation_id
        );

    if (details.is_null()) {
        error.set_null("details");
    } else {
        error.set_value(
            "details",
            std::move(details)
        );
    }

    auto document = JsonValue::object();

    document.set_value(
        "error",
        std::move(error)
    );

    auto response = make_json_response(
        status,
        document.serialize()
    );

    set_correlation_id_header(
        response,
        correlation_id
    );

    return response;
}

} // namespace

HttpResponse make_api_error_response(
    const HttpStatusCode status,
    const std::string_view code,
    const std::string_view message,
    const std::string_view correlation_id
)
{
    const auto resolved =
        ensure_correlation_id(correlation_id);

    return make_error_response(
        status,
        code,
        message,
        resolved,
        JsonValue::null()
    );
}

HttpResponse make_api_error_response(
    const HttpStatusCode status,
    const std::string_view code,
    const std::string_view message,
    const std::string_view correlation_id,
    JsonValue details
)
{
    const auto resolved =
        ensure_correlation_id(correlation_id);

    return make_error_response(
        status,
        code,
        message,
        resolved,
        std::move(details)
    );
}

} // namespace dispatcher::http
