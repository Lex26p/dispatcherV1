#pragma once

#include "scada_http/http_types.h"

#include <cstddef>
#include <string>
#include <string_view>

namespace dispatcher::http {

inline constexpr std::string_view correlation_id_header_name{
    "X-Correlation-Id"
};

inline constexpr std::size_t maximum_correlation_id_length = 64;

[[nodiscard]] bool is_valid_correlation_id(
    std::string_view value
) noexcept;

[[nodiscard]] std::string make_correlation_id();

[[nodiscard]] std::string ensure_correlation_id(
    std::string_view candidate
);

[[nodiscard]] std::string resolve_correlation_id(
    const HttpRequest& request
);

void set_correlation_id_header(
    HttpResponse& response,
    std::string_view correlation_id
);

} // namespace dispatcher::http
