#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace dispatcher::http {

struct HttpQueryParameter {
    std::string name;
    std::string value;
};

struct HttpQueryParameters {
    std::vector<HttpQueryParameter> values;

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;

    [[nodiscard]] bool contains(
        std::string_view name
    ) const noexcept;

    [[nodiscard]] std::optional<std::string_view> value(
        std::string_view name
    ) const noexcept;
};

struct HttpQueryError {
    std::string field;
    std::string reason;
};

struct HttpQueryParseResult {
    HttpQueryParameters parameters;
    std::optional<HttpQueryError> error;

    [[nodiscard]] bool is_valid() const noexcept;
};

[[nodiscard]] HttpQueryParseResult parse_http_query(
    std::string_view query,
    std::span<const std::string_view> allowed_parameters
);

[[nodiscard]] bool parse_http_query_bool(
    std::string_view value,
    bool& result
) noexcept;

[[nodiscard]] bool parse_http_query_uint32(
    std::string_view value,
    std::uint32_t& result
) noexcept;

} // namespace dispatcher::http
