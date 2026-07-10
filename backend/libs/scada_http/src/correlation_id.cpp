#include "scada_http/correlation_id.h"

#include <cstdint>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

namespace dispatcher::http {
namespace {

[[nodiscard]] bool is_ascii_alpha_numeric(
    const unsigned char character
) noexcept
{
    return (
        character >= static_cast<unsigned char>('a')
        && character <= static_cast<unsigned char>('z')
    ) || (
        character >= static_cast<unsigned char>('A')
        && character <= static_cast<unsigned char>('Z')
    ) || (
        character >= static_cast<unsigned char>('0')
        && character <= static_cast<unsigned char>('9')
    );
}

[[nodiscard]] bool is_allowed_character(
    const unsigned char character
) noexcept
{
    return is_ascii_alpha_numeric(character)
        || character == static_cast<unsigned char>('-')
        || character == static_cast<unsigned char>('_')
        || character == static_cast<unsigned char>('.');
}

[[nodiscard]] unsigned char to_ascii_lower(
    const unsigned char character
) noexcept
{
    if (
        character >= static_cast<unsigned char>('A')
        && character <= static_cast<unsigned char>('Z')
    ) {
        return static_cast<unsigned char>(
            character
            + static_cast<unsigned char>('a' - 'A')
        );
    }

    return character;
}

[[nodiscard]] bool equals_ignore_ascii_case(
    const std::string_view left,
    const std::string_view right
) noexcept
{
    if (left.size() != right.size()) {
        return false;
    }

    for (std::size_t index = 0; index < left.size(); ++index) {
        if (
            to_ascii_lower(
                static_cast<unsigned char>(left[index])
            )
            != to_ascii_lower(
                static_cast<unsigned char>(right[index])
            )
        ) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] std::uint64_t random_uint64()
{
    thread_local std::mt19937_64 generator{
        std::random_device{}()
    };

    return generator();
}

} // namespace

bool is_valid_correlation_id(
    const std::string_view value
) noexcept
{
    if (
        value.empty()
        || value.size() > maximum_correlation_id_length
    ) {
        return false;
    }

    for (const auto character : value) {
        if (
            !is_allowed_character(
                static_cast<unsigned char>(character)
            )
        ) {
            return false;
        }
    }

    return true;
}

std::string make_correlation_id()
{
    std::ostringstream output;

    output
        << std::hex
        << std::nouppercase
        << std::setfill('0')
        << std::setw(16)
        << random_uint64()
        << std::setw(16)
        << random_uint64();

    return output.str();
}

std::string ensure_correlation_id(
    const std::string_view candidate
)
{
    if (is_valid_correlation_id(candidate)) {
        return std::string{candidate};
    }

    return make_correlation_id();
}

std::string resolve_correlation_id(
    const HttpRequest& request
)
{
    if (is_valid_correlation_id(request.correlation_id)) {
        return request.correlation_id;
    }

    for (const auto& header : request.headers) {
        if (
            equals_ignore_ascii_case(
                header.name,
                correlation_id_header_name
            )
        ) {
            return ensure_correlation_id(
                header.value
            );
        }
    }

    return make_correlation_id();
}

void set_correlation_id_header(
    HttpResponse& response,
    const std::string_view correlation_id
)
{
    const auto resolved =
        ensure_correlation_id(correlation_id);

    for (auto& header : response.headers) {
        if (
            equals_ignore_ascii_case(
                header.name,
                correlation_id_header_name
            )
        ) {
            header.name = std::string{
                correlation_id_header_name
            };

            header.value = resolved;
            return;
        }
    }

    response.headers.push_back(
        HttpHeader{
            .name = std::string{
                correlation_id_header_name
            },
            .value = resolved
        }
    );
}

} // namespace dispatcher::http
