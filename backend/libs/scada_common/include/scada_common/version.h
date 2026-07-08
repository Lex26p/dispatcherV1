#pragma once

#include <string_view>

namespace dispatcher::common
{
    struct VersionInfo
    {
        std::string_view product_name;
        std::string_view executable_name;
        std::string_view version;
        std::string_view mode;
    };

    VersionInfo get_version_info();
}