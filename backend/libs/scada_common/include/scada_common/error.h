#pragma once

#include <string>
#include <string_view>

namespace dispatcher::common
{
    enum class ErrorCode
    {
        None,
        Unknown,
        InvalidArgument,
        NotFound,
        AlreadyExists,
        Conflict,
        Timeout,
        CommunicationError,
        PermissionDenied,
        ValidationFailed,
        InternalError,
        NotImplemented
    };

    std::string_view to_string(ErrorCode code);

    struct Error
    {
        ErrorCode code = ErrorCode::None;
        std::string message;

        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] bool has_error() const noexcept;
    };

    Error make_error(ErrorCode code, std::string_view message);
}