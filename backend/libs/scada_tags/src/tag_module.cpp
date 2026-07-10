#include "scada_tags/tag_module.h"

namespace dispatcher::tags
{
    dispatcher::core::ModuleInfo get_tag_module_info()
    {
        return dispatcher::core::ModuleInfo{
            .code = "scada_tags",
            .name = "Tags",
            .description = "Tag model, addressing and archive policy configuration.",
            .status = dispatcher::core::ModuleStatus::Running
        };
    }
}
