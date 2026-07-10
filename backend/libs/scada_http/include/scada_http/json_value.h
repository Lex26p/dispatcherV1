#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

namespace dispatcher::http {

class JsonValue final {
public:
    [[nodiscard]] static JsonValue null();
    [[nodiscard]] static JsonValue object();
    [[nodiscard]] static JsonValue array();

    JsonValue(JsonValue&& other) noexcept;
    JsonValue& operator=(JsonValue&& other) noexcept;

    JsonValue(const JsonValue&) = delete;
    JsonValue& operator=(const JsonValue&) = delete;

    ~JsonValue();

    [[nodiscard]] bool is_null() const noexcept;
    [[nodiscard]] bool is_object() const noexcept;
    [[nodiscard]] bool is_array() const noexcept;

    JsonValue& set_null(
        std::string_view key
    );

    JsonValue& set_string(
        std::string_view key,
        std::string_view value
    );

    JsonValue& set_bool(
        std::string_view key,
        bool value
    );

    JsonValue& set_int64(
        std::string_view key,
        std::int64_t value
    );

    JsonValue& set_uint64(
        std::string_view key,
        std::uint64_t value
    );

    JsonValue& set_double(
        std::string_view key,
        double value
    );

    JsonValue& set_value(
        std::string_view key,
        JsonValue value
    );

    JsonValue& append_null();

    JsonValue& append_string(
        std::string_view value
    );

    JsonValue& append_bool(
        bool value
    );

    JsonValue& append_int64(
        std::int64_t value
    );

    JsonValue& append_uint64(
        std::uint64_t value
    );

    JsonValue& append_double(
        double value
    );

    JsonValue& append_value(
        JsonValue value
    );

    [[nodiscard]] std::string serialize() const;

private:
    struct Impl;

    explicit JsonValue(
        std::unique_ptr<Impl> impl
    );

    [[nodiscard]] Impl& require_impl();
    [[nodiscard]] const Impl& require_impl() const;

    void require_object() const;
    void require_array() const;

    std::unique_ptr<Impl> impl_;
};

} // namespace dispatcher::http
