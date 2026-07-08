#include "scada_common/timestamp.h"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace dispatcher::common
{
    Timestamp now_utc()
    {
        return SystemClock::now();
    }

    std::string to_iso8601_utc(Timestamp timestamp)
    {
        const auto time = SystemClock::to_time_t(timestamp);

        std::tm utc_time{};

#if defined(_WIN32)
        gmtime_s(&utc_time, &time);
#else
        gmtime_r(&time, &utc_time);
#endif

        std::ostringstream stream;
        stream << std::put_time(&utc_time, "%Y-%m-%dT%H:%M:%SZ");
        return stream.str();
    }
}