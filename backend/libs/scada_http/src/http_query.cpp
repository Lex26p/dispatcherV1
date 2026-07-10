#include "scada_http/http_query.h"

#include <algorithm>
#include <charconv>
#include <limits>
#include <string>
#include <system_error>

namespace dispatcher::http {
namespace {

[[nodiscard]] int hex_value(
    const char character
) noexcept
{
    if (
        character >= '0'
        && character <= '9'
    ) {
        return character - '0';
    }

    if (
        character >= 'a'
        && character <= 'f'
    ) {
        return character - 'a' + 10;
    }

    if (
        character >= 'A'
        && character <= 'F'
    ) {
        return character - 'A' + 10;
    }

    return -1;
}

[[nodiscard]] bool decode_component(
    const std::string_view input,
    std::string& output
)
{
    output.clear();
    output.reserve(input.size());

    for (
        std::size_t index = 0;
        index < input.size();
        ++index
    ) {
        const auto character = input[index];

        if (character == '+') {
            output.push_back(' ');
            continue;
        }

        if (character != '%') {
            output.push_back(character);
            continue;
        }

        if (index + 2 >= input.size()) {
            return false;
        }

        const auto high =
            hex_value(input[index + 1]);

        const auto low =
            hex_value(input[index + 2]);

        if (high < 0 || low < 0) {
            return false;
        }

        output.push_back(
            static_cast<char>(
                high * 16 + low
            )
        );

        index += 2;
    }

    return true;
}

[[nodiscard]] bool is_allowed_parameter(
    const std::string_view name,
    const std::span<
        const std::string_view
    > allowed_parameters
)
{
    return std::ranges::find(
        allowed_parameters,
        name
    ) != allowed_parameters.end();
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

    for (
        std::size_t index = 0;
        index < left.size();
        ++index
    ) {
        if (
            to_ascii_lower(
                static_cast<unsigned char>(
                    left[index]
                )
            )
            != to_ascii_lower(
                static_cast<unsigned char>(
                    right[index]
                )
            )
        ) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] HttpQueryParseResult
make_query_error(
    std::string field,
    std::string reason
)
{
    HttpQueryParseResult result;

    result.error = HttpQueryError{
        .field = std::move(field),
        .reason = std::move(reason)
    };

    return result;
}

} // namespace

bool HttpQueryParameters::empty() const noexcept
{
    return values.empty();
}

std::size_t HttpQueryParameters::size() const noexcept
{
    return values.size();
}

bool HttpQueryParameters::contains(
    const std::string_view name
) const noexcept
{
    return value(name).has_value();
}

std::optional<std::string_view>
HttpQueryParameters::value(
    const std::string_view name
) const noexcept
{
    for (const auto& parameter : values) {
        if (parameter.name == name) {
            return parameter.value;
        }
    }

    return std::nullopt;
}

bool HttpQueryParseResult::is_valid() const noexcept
{
    return !error.has_value();
}

HttpQueryParseResult parse_http_query(
    std::string_view query,
    const std::span<
        const std::string_view
    > allowed_parameters
)
{
    HttpQueryParseResult result;

    if (
        !query.empty()
        && query.front() == '?'
    ) {
        query.remove_prefix(1);
    }

    if (query.empty()) {
        return result;
    }

    std::size_t start = 0;

    while (start <= query.size()) {
        const auto separator =
            query.find('&', start);

        const auto end =
            separator == std::string_view::npos
                ? query.size()
                : separator;

        const auto raw_parameter =
            query.substr(
                start,
                end - start
            );

        if (raw_parameter.empty()) {
            return make_query_error(
                "query",
                "empty_parameter"
            );
        }

        const auto equals =
            raw_parameter.find('=');

        const auto raw_name =
            raw_parameter.substr(
                0,
                equals
            );

        const auto raw_value =
            equals == std::string_view::npos
                ? std::string_view{}
                : raw_parameter.substr(
                    equals + 1
                );

        std::string name;
        std::string value;

        if (
            !decode_component(
                raw_name,
                name
            )
            || !decode_component(
                raw_value,
                value
            )
        ) {
            return make_query_error(
                "query",
                "invalid_encoding"
            );
        }

        if (name.empty()) {
            return make_query_error(
                "query",
                "empty_name"
            );
        }

        if (
            !is_allowed_parameter(
                name,
                allowed_parameters
            )
        ) {
            return make_query_error(
                name,
                "unsupported_parameter"
            );
        }

        if (result.parameters.contains(name)) {
            return make_query_error(
                name,
                "duplicate_parameter"
            );
        }

        result.parameters.values.push_back(
            HttpQueryParameter{
                .name = std::move(name),
                .value = std::move(value)
            }
        );

        if (
            separator
            == std::string_view::npos
        ) {
            break;
        }

        start = separator + 1;
    }

    return result;
}

bool parse_http_query_bool(
    const std::string_view value,
    bool& result
) noexcept
{
    if (
        value == "1"
        || equals_ignore_ascii_case(
            value,
            "true"
        )
    ) {
        result = true;
        return true;
    }

    if (
        value == "0"
        || equals_ignore_ascii_case(
            value,
            "false"
        )
    ) {
        result = false;
        return true;
    }

    return false;
}

bool parse_http_query_uint32(
    const std::string_view value,
    std::uint32_t& result
) noexcept
{
    if (value.empty()) {
        return false;
    }

    std::uint64_t parsed = 0;

    const auto conversion =
        std::from_chars(
            value.data(),
            value.data() + value.size(),
            parsed
        );

    if (
        conversion.ec != std::errc{}
        || conversion.ptr
            != value.data() + value.size()
        || parsed
            > std::numeric_limits<
                std::uint32_t
            >::max()
    ) {
        return false;
    }

    result =
        static_cast<std::uint32_t>(
            parsed
        );

    return true;
}

} // namespace dispatcher::http
