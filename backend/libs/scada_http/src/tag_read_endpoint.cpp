#include "scada_http/tag_read_endpoint.h"

#include "scada_http/api_error.h"
#include "scada_http/http_query.h"
#include "scada_http/json_value.h"

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

namespace dispatcher::http {
namespace {

using dispatcher::api::ReadCollection;
using dispatcher::api::TagReadModel;
using dispatcher::api::TagReadQuery;

struct TagQueryParseResult {
    TagReadQuery query;
    std::optional<HttpQueryError> error;

    [[nodiscard]] bool is_valid() const noexcept
    {
        return !error.has_value();
    }
};

[[nodiscard]] TagQueryParseResult
make_tag_query_error(
    std::string field,
    std::string reason
)
{
    TagQueryParseResult result;

    result.error = HttpQueryError{
        .field = std::move(field),
        .reason = std::move(reason)
    };

    return result;
}

[[nodiscard]] TagQueryParseResult
parse_tag_query(
    const std::string_view query_string
)
{
    static constexpr std::array<
        std::string_view,
        6
    > allowed_parameters{
        "id",
        "objectId",
        "deviceId",
        "includeDisabled",
        "limit",
        "offset"
    };

    const auto parsed =
        parse_http_query(
            query_string,
            allowed_parameters
        );

    if (!parsed.is_valid()) {
        TagQueryParseResult result;
        result.error = parsed.error;
        return result;
    }

    TagQueryParseResult result;

    if (
        const auto id =
            parsed.parameters.value("id");
        id.has_value()
    ) {
        if (id->empty()) {
            return make_tag_query_error(
                "id",
                "must_not_be_empty"
            );
        }

        result.query.filter.id =
            std::string{*id};
    }

    if (
        const auto object_id =
            parsed.parameters.value(
                "objectId"
            );
        object_id.has_value()
    ) {
        if (object_id->empty()) {
            return make_tag_query_error(
                "objectId",
                "must_not_be_empty"
            );
        }

        result.query.filter.object_id =
            std::string{*object_id};
    }

    if (
        const auto device_id =
            parsed.parameters.value(
                "deviceId"
            );
        device_id.has_value()
    ) {
        if (device_id->empty()) {
            return make_tag_query_error(
                "deviceId",
                "must_not_be_empty"
            );
        }

        result.query.filter.device_id =
            std::string{*device_id};
    }

    if (
        const auto include_disabled =
            parsed.parameters.value(
                "includeDisabled"
            );
        include_disabled.has_value()
    ) {
        bool value = false;

        if (
            !parse_http_query_bool(
                *include_disabled,
                value
            )
        ) {
            return make_tag_query_error(
                "includeDisabled",
                "invalid_boolean"
            );
        }

        result.query.options.include_disabled =
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
            return make_tag_query_error(
                "limit",
                "invalid_integer"
            );
        }

        if (value == 0) {
            return make_tag_query_error(
                "limit",
                "must_be_positive"
            );
        }

        if (
            value
            > result.query.options.max_limit
        ) {
            return make_tag_query_error(
                "limit",
                "exceeds_maximum"
            );
        }

        result.query.options.limit = value;
    }

    if (
        const auto offset =
            parsed.parameters.value("offset");
        offset.has_value()
    ) {
        std::uint32_t value = 0;

        if (
            !parse_http_query_uint32(
                *offset,
                value
            )
        ) {
            return make_tag_query_error(
                "offset",
                "invalid_integer"
            );
        }

        result.query.options.offset = value;
    }

    return result;
}

[[nodiscard]] JsonValue make_tag_json(
    const TagReadModel& tag
)
{
    auto result = JsonValue::object();

    result.set_string(
        "id",
        tag.id
    );

    if (tag.object_id.has_value()) {
        result.set_string(
            "objectId",
            tag.object_id.value()
        );
    } else {
        result.set_null("objectId");
    }

    if (tag.device_id.has_value()) {
        result.set_string(
            "deviceId",
            tag.device_id.value()
        );
    } else {
        result.set_null("deviceId");
    }

    result
        .set_string(
            "type",
            tag.type
        )
        .set_string(
            "valueType",
            tag.value_type
        )
        .set_string(
            "archivePolicy",
            tag.archive_policy
        )
        .set_string(
            "code",
            tag.code
        )
        .set_string(
            "name",
            tag.name
        )
        .set_string(
            "description",
            tag.description
        )
        .set_string(
            "engineeringUnit",
            tag.engineering_unit
        )
        .set_double(
            "scale",
            tag.scale
        )
        .set_double(
            "offset",
            tag.offset
        )
        .set_string(
            "address",
            tag.address
        )
        .set_bool(
            "enabled",
            tag.enabled
        );

    return result;
}

[[nodiscard]] std::string serialize_tags(
    const ReadCollection<TagReadModel>& collection
)
{
    auto tags = JsonValue::array();

    for (const auto& tag : collection.items) {
        tags.append_value(
            make_tag_json(tag)
        );
    }

    auto document = JsonValue::object();

    document
        .set_value(
            "tags",
            std::move(tags)
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
handle_tags_request(
    const HttpRequest& request,
    const dispatcher::api::IConfigurationReadService&
        read_service
)
{
    const auto parsed =
        parse_tag_query(
            request.query
        );

    if (!parsed.is_valid()) {
        return make_invalid_query_response(
            parsed.error.value(),
            request.correlation_id
        );
    }

    const auto result =
        read_service.read_tags(
            parsed.query
        );

    return make_json_response(
        HttpStatusCode::Ok,
        serialize_tags(result)
    );
}

} // namespace

std::string_view
tags_endpoint_path() noexcept
{
    return "/api/tags";
}

HttpEndpoint make_tags_endpoint()
{
    return HttpEndpoint{
        .method = HttpMethod::Get,
        .path = std::string{
            tags_endpoint_path()
        },
        .name = "Tags",
        .public_endpoint = true
    };
}

bool register_tag_read_endpoint(
    HttpRouteDispatcher& route_dispatcher,
    dispatcher::api::ConfigurationReadServicePtr read_service
)
{
    if (read_service == nullptr) {
        return false;
    }

    return route_dispatcher.add_route(
        make_tags_endpoint(),
        [
            read_service
        ](
            const HttpRequest& request
        ) {
            return handle_tags_request(
                request,
                *read_service
            );
        }
    );
}

} // namespace dispatcher::http
