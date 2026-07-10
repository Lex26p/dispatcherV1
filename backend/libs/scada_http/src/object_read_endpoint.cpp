#include "scada_http/object_read_endpoint.h"

#include "scada_http/api_error.h"
#include "scada_http/http_query.h"
#include "scada_http/json_value.h"

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

namespace dispatcher::http {
namespace {

using dispatcher::api::ObjectReadModel;
using dispatcher::api::ObjectReadQuery;
using dispatcher::api::ObjectTreeNodeReadModel;
using dispatcher::api::ObjectTreeReadResult;
using dispatcher::api::ReadCollection;

struct ObjectQueryParseResult {
    ObjectReadQuery query;
    std::optional<HttpQueryError> error;

    [[nodiscard]] bool is_valid() const noexcept
    {
        return !error.has_value();
    }
};

[[nodiscard]] ObjectQueryParseResult
make_object_query_error(
    std::string field,
    std::string reason
)
{
    ObjectQueryParseResult result;

    result.error = HttpQueryError{
        .field = std::move(field),
        .reason = std::move(reason)
    };

    return result;
}

[[nodiscard]] ObjectQueryParseResult
parse_object_query(
    const std::string_view query_string,
    const bool tree_endpoint
)
{
    static constexpr std::array<
        std::string_view,
        5
    > list_parameters{
        "id",
        "parentId",
        "includeChildren",
        "limit",
        "offset"
    };

    static constexpr std::array<
        std::string_view,
        2
    > tree_parameters{
        "id",
        "parentId"
    };

    const auto allowed_parameters =
        tree_endpoint
            ? std::span<
                const std::string_view
            >{tree_parameters}
            : std::span<
                const std::string_view
            >{list_parameters};

    const auto parsed =
        parse_http_query(
            query_string,
            allowed_parameters
        );

    if (!parsed.is_valid()) {
        ObjectQueryParseResult result;
        result.error = parsed.error;
        return result;
    }

    ObjectQueryParseResult result;

    if (
        const auto id =
            parsed.parameters.value("id");
        id.has_value()
    ) {
        if (id->empty()) {
            return make_object_query_error(
                "id",
                "must_not_be_empty"
            );
        }

        result.query.filter.id =
            std::string{*id};
    }

    if (
        const auto parent_id =
            parsed.parameters.value(
                "parentId"
            );
        parent_id.has_value()
    ) {
        if (parent_id->empty()) {
            return make_object_query_error(
                "parentId",
                "must_not_be_empty"
            );
        }

        result.query.parent_id =
            std::string{*parent_id};
    }

    if (
        const auto include_children =
            parsed.parameters.value(
                "includeChildren"
            );
        include_children.has_value()
    ) {
        bool value = false;

        if (
            !parse_http_query_bool(
                *include_children,
                value
            )
        ) {
            return make_object_query_error(
                "includeChildren",
                "invalid_boolean"
            );
        }

        result.query.options.include_children =
            value;
    }

    if (
        const auto limit =
            parsed.parameters.value("limit");
        limit.has_value()
    ) {
        std::uint32_t value = 0;

        if (
            !parse_http_query_uint32(
                *limit,
                value
            )
        ) {
            return make_object_query_error(
                "limit",
                "invalid_integer"
            );
        }

        if (value == 0) {
            return make_object_query_error(
                "limit",
                "must_be_positive"
            );
        }

        if (
            value
            > result.query.options.max_limit
        ) {
            return make_object_query_error(
                "limit",
                "exceeds_maximum"
            );
        }

        result.query.options.limit =
            value;
    }

    if (
        const auto offset =
            parsed.parameters.value(
                "offset"
            );
        offset.has_value()
    ) {
        std::uint32_t value = 0;

        if (
            !parse_http_query_uint32(
                *offset,
                value
            )
        ) {
            return make_object_query_error(
                "offset",
                "invalid_integer"
            );
        }

        result.query.options.offset =
            value;
    }

    return result;
}

[[nodiscard]] JsonValue make_object_json(
    const ObjectReadModel& object
)
{
    auto result = JsonValue::object();

    result.set_string(
        "id",
        object.id
    );

    if (object.parent_id.has_value()) {
        result.set_string(
            "parentId",
            object.parent_id.value()
        );
    } else {
        result.set_null("parentId");
    }

    result
        .set_string(
            "type",
            object.type
        )
        .set_string(
            "code",
            object.code
        )
        .set_string(
            "name",
            object.name
        )
        .set_string(
            "description",
            object.description
        )
        .set_bool(
            "isRoot",
            object.is_root
        );

    return result;
}

[[nodiscard]] JsonValue make_tree_node_json(
    const ObjectTreeNodeReadModel& node
)
{
    auto result = JsonValue::object();

    result.set_string(
        "id",
        node.id
    );

    if (node.parent_id.has_value()) {
        result.set_string(
            "parentId",
            node.parent_id.value()
        );
    } else {
        result.set_null("parentId");
    }

    result
        .set_string(
            "type",
            node.type
        )
        .set_string(
            "code",
            node.code
        )
        .set_string(
            "name",
            node.name
        )
        .set_string(
            "description",
            node.description
        );

    auto children = JsonValue::array();

    for (const auto& child : node.children) {
        children.append_value(
            make_tree_node_json(child)
        );
    }

    result.set_value(
        "children",
        std::move(children)
    );

    return result;
}

[[nodiscard]] std::string serialize_objects(
    const ReadCollection<ObjectReadModel>&
        collection
)
{
    auto objects = JsonValue::array();

    for (const auto& object : collection.items) {
        objects.append_value(
            make_object_json(object)
        );
    }

    auto document = JsonValue::object();

    document
        .set_value(
            "objects",
            std::move(objects)
        )
        .set_uint64(
            "count",
            static_cast<std::uint64_t>(
                collection.count()
            )
        )
        .set_uint64(
            "total",
            collection.total
        )
        .set_uint64(
            "offset",
            collection.offset
        )
        .set_uint64(
            "limit",
            collection.limit
        )
        .set_bool(
            "hasMore",
            collection.has_more
        );

    return document.serialize();
}

[[nodiscard]] std::string serialize_object_tree(
    const ObjectTreeReadResult& tree
)
{
    auto roots = JsonValue::array();

    for (const auto& root : tree.roots) {
        roots.append_value(
            make_tree_node_json(root)
        );
    }

    auto document = JsonValue::object();

    document
        .set_value(
            "roots",
            std::move(roots)
        )
        .set_uint64(
            "count",
            tree.count
        );

    return document.serialize();
}

[[nodiscard]] HttpResponse
make_invalid_query_response(
    const HttpQueryError& error,
    const std::string_view correlation_id
)
{
    auto details = JsonValue::object();

    details
        .set_string(
            "field",
            error.field
        )
        .set_string(
            "reason",
            error.reason
        );

    return make_api_error_response(
        HttpStatusCode::BadRequest,
        "invalid_query",
        "The query string is invalid.",
        correlation_id,
        std::move(details)
    );
}

[[nodiscard]] HttpResponse
handle_objects_request(
    const HttpRequest& request,
    const dispatcher::api::
        IConfigurationReadService&
            read_service
)
{
    const auto parsed =
        parse_object_query(
            request.query,
            false
        );

    if (!parsed.is_valid()) {
        return make_invalid_query_response(
            parsed.error.value(),
            request.correlation_id
        );
    }

    const auto result =
        read_service.read_objects(
            parsed.query
        );

    return make_json_response(
        HttpStatusCode::Ok,
        serialize_objects(result)
    );
}

[[nodiscard]] HttpResponse
handle_object_tree_request(
    const HttpRequest& request,
    const dispatcher::api::
        IConfigurationReadService&
            read_service
)
{
    const auto parsed =
        parse_object_query(
            request.query,
            true
        );

    if (!parsed.is_valid()) {
        return make_invalid_query_response(
            parsed.error.value(),
            request.correlation_id
        );
    }

    const auto result =
        read_service.read_object_tree(
            parsed.query
        );

    return make_json_response(
        HttpStatusCode::Ok,
        serialize_object_tree(result)
    );
}

} // namespace

std::string_view
objects_endpoint_path() noexcept
{
    return "/api/objects";
}

std::string_view
object_tree_endpoint_path() noexcept
{
    return "/api/objects/tree";
}

HttpEndpoint make_objects_endpoint()
{
    return HttpEndpoint{
        .method = HttpMethod::Get,
        .path = std::string{
            objects_endpoint_path()
        },
        .name = "Objects",
        .public_endpoint = true
    };
}

HttpEndpoint make_object_tree_endpoint()
{
    return HttpEndpoint{
        .method = HttpMethod::Get,
        .path = std::string{
            object_tree_endpoint_path()
        },
        .name = "Object tree",
        .public_endpoint = true
    };
}

bool register_object_read_endpoints(
    HttpRouteDispatcher& route_dispatcher,
    dispatcher::api::ConfigurationReadServicePtr
        read_service
)
{
    if (read_service == nullptr) {
        return false;
    }

    const auto objects_registered =
        route_dispatcher.add_route(
            make_objects_endpoint(),
            [
                read_service
            ](
                const HttpRequest& request
            ) {
                return handle_objects_request(
                    request,
                    *read_service
                );
            }
        );

    const auto tree_registered =
        route_dispatcher.add_route(
            make_object_tree_endpoint(),
            [
                read_service
            ](
                const HttpRequest& request
            ) {
                return handle_object_tree_request(
                    request,
                    *read_service
                );
            }
        );

    return objects_registered
        && tree_registered;
}

} // namespace dispatcher::http
