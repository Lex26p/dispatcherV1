#pragma once

#include <string>
#include <string_view>

namespace dispatcher::api
{
    enum class ApiResponseStatus
    {
        Ok,
        Created,
        Accepted,
        NoContent,
        BadRequest,
        Unauthorized,
        Forbidden,
        NotFound,
        Conflict,
        ValidationError,
        InternalError,
        NotImplemented
    };

    struct ApiResponse
    {
        ApiResponseStatus status = ApiResponseStatus::Ok;

        bool success = true;

        int http_status_code = 200;

        std::string correlation_id;
        std::string message;
        std::string body;

        [[nodiscard]] bool is_success() const noexcept;
        [[nodiscard]] bool has_correlation_id() const noexcept;
        [[nodiscard]] bool has_message() const noexcept;
        [[nodiscard]] bool has_body() const noexcept;
    };

    [[nodiscard]] ApiResponse make_success_response(
        std::string body = {},
        std::string message = {}
    );

    [[nodiscard]] ApiResponse make_error_response(
        ApiResponseStatus status,
        std::string message,
        std::string body = {}
    );

    [[nodiscard]] int default_http_status_code(
        ApiResponseStatus status
    ) noexcept;

    [[nodiscard]] std::string_view to_string(
        ApiResponseStatus status
    ) noexcept;
}
