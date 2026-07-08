#include "scada_common/error.h"

namespace dispatcher::common
{
    std::string_view to_string(ErrorCode code)
    {
        switch (code)
        {
        case ErrorCode::None:
            return "None";
        case ErrorCode::Unknown:
            return "Unknown";
        case ErrorCode::InvalidArgument:
            return "InvalidArgument";
        case ErrorCode::NotFound:
            return "NotFound";
        case ErrorCode::AlreadyExists:
            return "AlreadyExists";
        case ErrorCode::Conflict:
            return "Conflict";
        case ErrorCode::Timeout:
            return "Timeout";
        case ErrorCode::CommunicationError:
            return "CommunicationError";
        case ErrorCode::PermissionDenied:
            return "PermissionDenied";
        case ErrorCode::ValidationFailed:
            return "ValidationFailed";
        case ErrorCode::InternalError:
            return "InternalError";
        case ErrorCode::NotImplemented:
            return "NotImplemented";
        }

        return "Unknown";
    }

    bool Error::empty() const noexcept
    {
        return code == ErrorCode::None && message.empty();
    }

    bool Error::has_error() const noexcept
    {
        return !empty();
    }

    Error make_error(ErrorCode code, std::string_view message)
    {
        return Error{
            .code = code,
            .message = std::string{message}
        };
    }
}