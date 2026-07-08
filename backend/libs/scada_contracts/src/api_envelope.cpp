#include "scada_contracts/api_envelope.h"

namespace dispatcher::contracts
{
    ApiEnvelope make_success_envelope(std::string_view correlation_id, std::string_view message)
    {
        return ApiEnvelope{
            .success = true,
            .correlation_id = std::string{correlation_id},
            .message = std::string{message}
        };
    }
}