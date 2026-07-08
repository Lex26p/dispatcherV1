#pragma once

#include "scada_common/error.h"

#include <optional>
#include <utility>

namespace dispatcher::common
{
    template <typename TValue>
    class Result
    {
    public:
        static Result success(TValue value)
        {
            Result result;
            result.value_ = std::move(value);
            return result;
        }

        static Result failure(Error error)
        {
            Result result;
            result.error_ = std::move(error);
            return result;
        }

        [[nodiscard]] bool is_success() const noexcept
        {
            return value_.has_value() && !error_.has_error();
        }

        [[nodiscard]] bool is_failure() const noexcept
        {
            return !is_success();
        }

        [[nodiscard]] const TValue& value() const
        {
            return value_.value();
        }

        [[nodiscard]] TValue& value()
        {
            return value_.value();
        }

        [[nodiscard]] const Error& error() const noexcept
        {
            return error_;
        }

    private:
        std::optional<TValue> value_;
        Error error_;
    };

    template <>
    class Result<void>
    {
    public:
        static Result success()
        {
            return Result{};
        }

        static Result failure(Error error)
        {
            Result result;
            result.error_ = std::move(error);
            return result;
        }

        [[nodiscard]] bool is_success() const noexcept
        {
            return !error_.has_error();
        }

        [[nodiscard]] bool is_failure() const noexcept
        {
            return !is_success();
        }

        [[nodiscard]] const Error& error() const noexcept
        {
            return error_;
        }

    private:
        Error error_;
    };
}