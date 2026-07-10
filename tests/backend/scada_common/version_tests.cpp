#include "scada_common/version.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE(
    "version info identifies Dispatcher product",
    "[scada_common][version]"
)
{
    const auto version_info =
        dispatcher::common::get_version_info();

    REQUIRE(version_info.product_name == "Dispatcher");
    REQUIRE(version_info.executable_name == "dispatcher_server");
}

TEST_CASE(
    "version info exposes required non-empty fields",
    "[scada_common][version]"
)
{
    const auto version_info =
        dispatcher::common::get_version_info();

    REQUIRE_FALSE(version_info.product_name.empty());
    REQUIRE_FALSE(version_info.executable_name.empty());
    REQUIRE_FALSE(version_info.version.empty());
    REQUIRE_FALSE(version_info.mode.empty());
}
