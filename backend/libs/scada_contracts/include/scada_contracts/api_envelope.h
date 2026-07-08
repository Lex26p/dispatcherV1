#pragma once

#include <string>
#include <string_view>

namespace dispatcher::contracts
{
    struct ApiEnvelope
    {
        bool success = true;
        std::string correlation_id;
        std::string message;
    };

    ApiEnvelope make_success_envelope(std::string_view correlation_id, std::string_view message);
}