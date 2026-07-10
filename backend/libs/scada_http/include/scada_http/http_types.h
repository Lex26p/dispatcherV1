#pragma once

#include <cstdint>

#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::http {

enum class HttpMethod {
    Get,
    Post,
    Put,
    Patch,
    Delete,
    Options,
    Head,
    Unknown
};

enum class HttpStatusCode : int {
    Ok = 200,
    Created = 201,
    Accepted = 202,
    NoContent = 204,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    Conflict = 409,
    PayloadTooLarge = 413,
    InternalServerError = 500,
    NotImplemented = 501,
    ServiceUnavailable = 503
};

struct HttpCorsOptions {
    bool enabled = true;
    bool allow_any_origin = false;
    bool allow_credentials = false;

    std::uint32_t max_age_seconds = 600;

    std::vector<std::string> allowed_origins{
        "http://localhost:5030",
        "http://127.0.0.1:5030",
        "https://localhost:5030"
    };

    std::vector<std::string> allowed_methods{
        "GET",
        "POST",
        "PUT",
        "PATCH",
        "DELETE",
        "OPTIONS"
    };

    std::vector<std::string> allowed_headers{
        "Accept",
        "Authorization",
        "Content-Type",
        "Origin",
        "X-Correlation-Id",
        "X-Requested-With"
    };

    [[nodiscard]] bool has_allowed_origins() const noexcept;
    [[nodiscard]] bool has_allowed_methods() const noexcept;
    [[nodiscard]] bool has_allowed_headers() const noexcept;
    [[nodiscard]] bool is_valid() const noexcept;
};

struct HttpServerOptions {
    std::string bind_address = "127.0.0.1";

    std::uint16_t port = 8080;
    std::uint32_t worker_threads = 1;

    bool reuse_address = true;
    bool start_on_launch = false;

    std::uint64_t request_body_limit_bytes = 1024 * 1024;

    std::string server_name = "Dispatcher";

    HttpCorsOptions cors;

    [[nodiscard]] bool has_valid_bind_address() const noexcept;
    [[nodiscard]] bool has_valid_port() const noexcept;
    [[nodiscard]] bool has_valid_worker_thread_count() const noexcept;
    [[nodiscard]] bool has_valid_request_body_limit() const noexcept;
    [[nodiscard]] bool is_valid() const noexcept;
};

struct HttpHeader {
    std::string name;
    std::string value;

    [[nodiscard]] bool is_valid() const noexcept;
};

struct HttpRequest {
    HttpMethod method = HttpMethod::Unknown;

    std::string target;
    std::string path;
    std::string query;

    std::vector<HttpHeader> headers;

    std::string correlation_id;
    std::string body;

    [[nodiscard]] bool has_target() const noexcept;
    [[nodiscard]] bool has_path() const noexcept;
    [[nodiscard]] bool has_body() const noexcept;
    [[nodiscard]] bool is_valid() const noexcept;
};

struct HttpResponse {
    HttpStatusCode status = HttpStatusCode::Ok;

    std::string content_type =
        "application/json; charset=utf-8";

    std::vector<HttpHeader> headers;

    std::string body;

    [[nodiscard]] int status_code() const noexcept;
    [[nodiscard]] bool has_body() const noexcept;
    [[nodiscard]] bool is_success() const noexcept;
};

struct HttpEndpoint {
    HttpMethod method = HttpMethod::Get;

    std::string path;
    std::string name;

    bool public_endpoint = true;

    [[nodiscard]] bool has_path() const noexcept;
    [[nodiscard]] bool has_name() const noexcept;
    [[nodiscard]] bool is_valid() const noexcept;
};

[[nodiscard]] std::string_view to_string(
    HttpMethod method
) noexcept;

[[nodiscard]] std::string_view to_string(
    HttpStatusCode status
) noexcept;

[[nodiscard]] HttpMethod http_method_from_string(
    std::string_view value
) noexcept;

[[nodiscard]] HttpResponse make_json_response(
    HttpStatusCode status,
    std::string body
);

[[nodiscard]] HttpResponse make_text_response(
    HttpStatusCode status,
    std::string body
);

[[nodiscard]] HttpResponse make_empty_response(
    HttpStatusCode status
);

} // namespace dispatcher::http
