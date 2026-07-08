#include "scada_common/version.h"

namespace dispatcher::common
{
    VersionInfo get_version_info()
    {
        return VersionInfo{
            .product_name = "Dispatcher",
            .executable_name = "dispatcher_server",
            .version = "0.1.0-dev",
            .mode = "Development"
        };
    }
}