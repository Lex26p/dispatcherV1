#include "scada_http/json_value.h"

#include <json/json.h>

#include <stdexcept>
#include <utility>

namespace dispatcher::http {

struct JsonValue::Impl {
    explicit Impl(
        Json::Value initial_value
    )
        : value(std::move(initial_value))
    {
    }

    Json::Value value;
};

JsonValue::JsonValue(
    std::unique_ptr<Impl> impl
)
    : impl_(std::move(impl))
{
}

JsonValue::JsonValue(
    JsonValue&& other
) noexcept = default;

JsonValue& JsonValue::operator=(
    JsonValue&& other
) noexcept = default;

JsonValue::~JsonValue() = default;

JsonValue JsonValue::null()
{
    return JsonValue{
        std::make_unique<Impl>(
            Json::Value{Json::nullValue}
        )
    };
}

JsonValue JsonValue::object()
{
    return JsonValue{
        std::make_unique<Impl>(
            Json::Value{Json::objectValue}
        )
    };
}

JsonValue JsonValue::array()
{
    return JsonValue{
        std::make_unique<Impl>(
            Json::Value{Json::arrayValue}
        )
    };
}

bool JsonValue::is_null() const noexcept
{
    return impl_ != nullptr
        && impl_->value.isNull();
}

bool JsonValue::is_object() const noexcept
{
    return impl_ != nullptr
        && impl_->value.isObject();
}

bool JsonValue::is_array() const noexcept
{
    return impl_ != nullptr
        && impl_->value.isArray();
}

JsonValue& JsonValue::set_null(
    const std::string_view key
)
{
    require_object();

    impl_->value[std::string{key}] =
        Json::Value{Json::nullValue};

    return *this;
}

JsonValue& JsonValue::set_string(
    const std::string_view key,
    const std::string_view value
)
{
    require_object();

    impl_->value[std::string{key}] =
        std::string{value};

    return *this;
}

JsonValue& JsonValue::set_bool(
    const std::string_view key,
    const bool value
)
{
    require_object();

    impl_->value[std::string{key}] = value;

    return *this;
}

JsonValue& JsonValue::set_int64(
    const std::string_view key,
    const std::int64_t value
)
{
    require_object();

    impl_->value[std::string{key}] =
        static_cast<Json::Int64>(value);

    return *this;
}

JsonValue& JsonValue::set_uint64(
    const std::string_view key,
    const std::uint64_t value
)
{
    require_object();

    impl_->value[std::string{key}] =
        static_cast<Json::UInt64>(value);

    return *this;
}

JsonValue& JsonValue::set_double(
    const std::string_view key,
    const double value
)
{
    require_object();

    impl_->value[std::string{key}] = value;

    return *this;
}

JsonValue& JsonValue::set_value(
    const std::string_view key,
    JsonValue value
)
{
    require_object();

    auto& nested = value.require_impl();

    impl_->value[std::string{key}] =
        std::move(nested.value);

    return *this;
}

JsonValue& JsonValue::append_null()
{
    require_array();

    impl_->value.append(
        Json::Value{Json::nullValue}
    );

    return *this;
}

JsonValue& JsonValue::append_string(
    const std::string_view value
)
{
    require_array();

    impl_->value.append(
        std::string{value}
    );

    return *this;
}

JsonValue& JsonValue::append_bool(
    const bool value
)
{
    require_array();

    impl_->value.append(value);

    return *this;
}

JsonValue& JsonValue::append_int64(
    const std::int64_t value
)
{
    require_array();

    impl_->value.append(
        static_cast<Json::Int64>(value)
    );

    return *this;
}

JsonValue& JsonValue::append_uint64(
    const std::uint64_t value
)
{
    require_array();

    impl_->value.append(
        static_cast<Json::UInt64>(value)
    );

    return *this;
}

JsonValue& JsonValue::append_double(
    const double value
)
{
    require_array();

    impl_->value.append(value);

    return *this;
}

JsonValue& JsonValue::append_value(
    JsonValue value
)
{
    require_array();

    auto& nested = value.require_impl();

    impl_->value.append(
        std::move(nested.value)
    );

    return *this;
}

std::string JsonValue::serialize() const
{
    const auto& implementation =
        require_impl();

    Json::StreamWriterBuilder builder;

    builder["commentStyle"] = "None";
    builder["indentation"] = "";

    return Json::writeString(
        builder,
        implementation.value
    );
}

JsonValue::Impl& JsonValue::require_impl()
{
    if (impl_ == nullptr) {
        throw std::logic_error{
            "JSON value is in moved-from state."
        };
    }

    return *impl_;
}

const JsonValue::Impl& JsonValue::require_impl() const
{
    if (impl_ == nullptr) {
        throw std::logic_error{
            "JSON value is in moved-from state."
        };
    }

    return *impl_;
}

void JsonValue::require_object() const
{
    const auto& implementation =
        require_impl();

    if (!implementation.value.isObject()) {
        throw std::logic_error{
            "JSON value is not an object."
        };
    }
}

void JsonValue::require_array() const
{
    const auto& implementation =
        require_impl();

    if (!implementation.value.isArray()) {
        throw std::logic_error{
            "JSON value is not an array."
        };
    }
}

} // namespace dispatcher::http
