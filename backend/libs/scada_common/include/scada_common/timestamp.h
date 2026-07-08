#pragma once

#include <chrono>
#include <string>

namespace dispatcher::common
{
    using SystemClock = std::chrono::system_clock;
    using Timestamp = std::chrono::time_point<SystemClock>;

    Timestamp now_utc();

    std::string to_iso8601_utc(Timestamp timestamp);
}