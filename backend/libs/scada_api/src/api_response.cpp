#include "scada_api/api_response.h"

#include <utility>

namespace dispatcher::api
{
    bool ApiResponse::is_success() const noexcept
    {
        return success;
    }

    bool ApiResponse::has_correlation_id() const noexcept
    {
        return !correlation_id.empty();
    }

    bool ApiResponse::has_message() const noexcept
    {
        return !message.empty();
    }

    bool ApiResponse::has_body() const noexcept
    {
        return !body.empty();
    }

    ApiResponse make_success_response(
        std::string body,
        std::string message
    )
    {
        ApiResponse response;

        response.status = ApiResponseStatus::Ok;
        response.success = true;
        response.http_status_code = default_http_status_code(response.status);
        response.body = std::move(body);
        response.message = std::move(message);

        return response;
    }

    ApiResponse make_error_response(
        ApiResponseStatus status,
        std::string message,
        std::string body
    )
    {
        ApiResponse response;

        response.status = status;
        response.success = false;
        response.http_status_code = default_http_status_code(status);
        response.message = std::move(message);
        response.body = std::move(body);

        return response;
    }

    int default_http_status_code(
        ApiResponseStatus status
    ) noexcept
    {
        switch (status)
        {
        case ApiResponseStatus::Ok:
            return 200;
        case ApiResponseStatus::Created:
            return 201;
        case ApiResponseStatus::Accepted:
            return 202;
        case ApiResponseStatus::NoContent:
            return 204;
        case ApiResponseStatus::BadRequest:
            return 400;
        case ApiResponseStatus::Unauthorized:
            return 401;
        case ApiResponseStatus::Forbidden:
            return 403;
        case ApiResponseStatus::NotFound:
            return 404;
        case ApiResponseStatus::Conflict:
            return 409;
        case ApiResponseStatus::ValidationError:
            return 422;
        case ApiResponseStatus::InternalError:
            return 500;
        case ApiResponseStatus::NotImplemented:
            return 501;
        }

        return 500;
    }

    std::string_view to_string(
        ApiResponseStatus status
    ) noexcept
    {
        switch (status)
        {
        case ApiResponseStatus::Ok:
            return "Ok";
        case ApiResponseStatus::Created:
            return "Created";
        case ApiResponseStatus::Accepted:
            return "Accepted";
        case ApiResponseStatus::NoContent:
            return "NoContent";
        case ApiResponseStatus::BadRequest:
            return "BadRequest";
        case ApiResponseStatus::Unauthorized:
            return "Unauthorized";
        case ApiResponseStatus::Forbidden:
            return "Forbidden";
        case ApiResponseStatus::NotFound:
            return "NotFound";
        case ApiResponseStatus::Conflict:
            return "Conflict";
        case ApiResponseStatus::ValidationError:
            return "ValidationError";
        case ApiResponseStatus::InternalError:
            return "InternalError";
        case ApiResponseStatus::NotImplemented:
            return "NotImplemented";
        }

        return "Unknown";
    }
}