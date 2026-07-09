#include "scada_http/http_types.h"

#include <algorithm>
#include <cctype>
#include <string>

namespace dispatcher::http {
namespace {

[[nodiscard]] std::string to_upper_copy(std::string_view value)
{
    std::string result(value);

    std::ranges::transform(
        result,
        result.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::toupper(character));
        }
    );

    return result;
}

} // namespace

bool HttpServerOptions::has_valid_bind_address() const noexcept
{
    return !bind_address.empty();
}

bool HttpServerOptions::has_valid_port() const noexcept
{
    return port > 0;
}

bool HttpServerOptions::has_valid_worker_thread_count() const noexcept
{
    return worker_threads > 0;
}

bool HttpServerOptions::has_valid_request_body_limit() const noexcept
{
    return request_body_limit_bytes > 0;
}

bool HttpServerOptions::is_valid() const noexcept
{
    return has_valid_bind_address()
        && has_valid_port()
        && has_valid_worker_thread_count()
        && has_valid_request_body_limit();
}

bool HttpHeader::is_valid() const noexcept
{
    return !name.empty();
}

bool HttpRequest::has_target() const noexcept
{
    return !target.empty();
}

bool HttpRequest::has_path() const noexcept
{
    return !path.empty();
}

bool HttpRequest::has_body() const noexcept
{
    return !body.empty();
}

bool HttpRequest::is_valid() const noexcept
{
    return method != HttpMethod::Unknown
        && has_target()
        && has_path();
}

int HttpResponse::status_code() const noexcept
{
    return static_cast<int>(status);
}

bool HttpResponse::has_body() const noexcept
{
    return !body.empty();
}

bool HttpResponse::is_success() const noexcept
{
    const auto code = status_code();

    return code >= 200 && code < 300;
}

bool HttpEndpoint::has_path() const noexcept
{
    return !path.empty();
}

bool HttpEndpoint::has_name() const noexcept
{
    return !name.empty();
}

bool HttpEndpoint::is_valid() const noexcept
{
    return method != HttpMethod::Unknown
        && has_path()
        && has_name();
}

std::string_view to_string(HttpMethod method) noexcept
{
    switch (method) {
    case HttpMethod::Get:
        return "GET";
    case HttpMethod::Post:
        return "POST";
    case HttpMethod::Put:
        return "PUT";
    case HttpMethod::Patch:
        return "PATCH";
    case HttpMethod::Delete:
        return "DELETE";
    case HttpMethod::Options:
        return "OPTIONS";
    case HttpMethod::Head:
        return "HEAD";
    case HttpMethod::Unknown:
    default:
        return "UNKNOWN";
    }
}

std::string_view to_string(HttpStatusCode status) noexcept
{
    switch (status) {
    case HttpStatusCode::Ok:
        return "OK";
    case HttpStatusCode::Created:
        return "Created";
    case HttpStatusCode::Accepted:
        return "Accepted";
    case HttpStatusCode::NoContent:
        return "No Content";
    case HttpStatusCode::BadRequest:
        return "Bad Request";
    case HttpStatusCode::Unauthorized:
        return "Unauthorized";
    case HttpStatusCode::Forbidden:
        return "Forbidden";
    case HttpStatusCode::NotFound:
        return "Not Found";
    case HttpStatusCode::MethodNotAllowed:
        return "Method Not Allowed";
    case HttpStatusCode::Conflict:
        return "Conflict";
    case HttpStatusCode::PayloadTooLarge:
        return "Payload Too Large";
    case HttpStatusCode::InternalServerError:
        return "Internal Server Error";
    case HttpStatusCode::NotImplemented:
        return "Not Implemented";
    case HttpStatusCode::ServiceUnavailable:
        return "Service Unavailable";
    default:
        return "Unknown";
    }
}

HttpMethod http_method_from_string(std::string_view value) noexcept
{
    const auto normalized = to_upper_copy(value);

    if (normalized == "GET") {
        return HttpMethod::Get;
    }

    if (normalized == "POST") {
        return HttpMethod::Post;
    }

    if (normalized == "PUT") {
        return HttpMethod::Put;
    }

    if (normalized == "PATCH") {
        return HttpMethod::Patch;
    }

    if (normalized == "DELETE") {
        return HttpMethod::Delete;
    }

    if (normalized == "OPTIONS") {
        return HttpMethod::Options;
    }

    if (normalized == "HEAD") {
        return HttpMethod::Head;
    }

    return HttpMethod::Unknown;
}

HttpResponse make_json_response(
    HttpStatusCode status,
    std::string body
)
{
    HttpResponse response;
    response.status = status;
    response.content_type = "application/json; charset=utf-8";
    response.body = std::move(body);

    return response;
}

HttpResponse make_text_response(
    HttpStatusCode status,
    std::string body
)
{
    HttpResponse response;
    response.status = status;
    response.content_type = "text/plain; charset=utf-8";
    response.body = std::move(body);

    return response;
}

HttpResponse make_empty_response(
    HttpStatusCode status
)
{
    HttpResponse response;
    response.status = status;
    response.content_type = "text/plain; charset=utf-8";

    return response;
}

} // namespace dispatcher::http
