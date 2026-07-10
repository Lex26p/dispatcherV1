#include "scada_http/object_read_endpoint.h"

#include "scada_app/development_configuration_read_service.h"

#include "scada_http/correlation_id.h"

#include <catch2/catch_test_macros.hpp>

#include <json/json.h>

#include <sstream>
#include <string>
#include <string_view>

namespace {

using dispatcher::http::HttpMethod;
using dispatcher::http::HttpRequest;
using dispatcher::http::HttpResponse;
using dispatcher::http::HttpRouteDispatcher;
using dispatcher::http::HttpStatusCode;
using dispatcher::http::correlation_id_header_name;
using dispatcher::http::object_tree_endpoint_path;
using dispatcher::http::objects_endpoint_path;
using dispatcher::http::register_object_read_endpoints;

[[nodiscard]] Json::Value parse_json(
    const std::string_view json
)
{
    Json::CharReaderBuilder builder;
    Json::Value result;
    std::string errors;

    std::istringstream input{
        std::string{json}
    };

    const auto parsed =
        Json::parseFromStream(
            builder,
            input,
            &result,
            &errors
        );

    REQUIRE(parsed);

    return result;
}

[[nodiscard]] HttpRequest make_request(
    std::string path,
    std::string query = {}
)
{
    HttpRequest request;

    request.method = HttpMethod::Get;
    request.path = std::move(path);
    request.query = std::move(query);
    request.target = request.path;

    if (!request.query.empty()) {
        request.target += "?";
        request.target += request.query;
    }

    request.correlation_id =
        "object-request-110";

    return request;
}

[[nodiscard]] std::string find_correlation_header(
    const HttpResponse& response
)
{
    for (const auto& header : response.headers) {
        if (
            header.name
            == correlation_id_header_name
        ) {
            return header.value;
        }
    }

    return {};
}

[[nodiscard]] HttpRouteDispatcher
make_dispatcher()
{
    HttpRouteDispatcher dispatcher;

    const auto service =
        dispatcher::app::
            create_development_configuration_read_service();

    REQUIRE(
        register_object_read_endpoints(
            dispatcher,
            service
        )
    );

    return dispatcher;
}

} // namespace

TEST_CASE(
    "Object endpoints expose stable paths and register routes",
    "[scada_http][objects]"
)
{
    REQUIRE(
        objects_endpoint_path()
        == "/api/objects"
    );

    REQUIRE(
        object_tree_endpoint_path()
        == "/api/objects/tree"
    );

    const auto dispatcher =
        make_dispatcher();

    REQUIRE(dispatcher.route_count() == 2);

    REQUIRE(
        dispatcher.has_route(
            HttpMethod::Get,
            "/api/objects"
        )
    );

    REQUIRE(
        dispatcher.has_route(
            HttpMethod::Get,
            "/api/objects/tree"
        )
    );
}

TEST_CASE(
    "Objects endpoint returns stable collection contract",
    "[scada_http][objects][json]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "/api/objects"
            )
        );

    REQUIRE(
        response.status
        == HttpStatusCode::Ok
    );

    REQUIRE(
        find_correlation_header(response)
        == "object-request-110"
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["objects"].isArray());
    REQUIRE(root["objects"].size() == 5);
    REQUIRE(root["count"].asUInt64() == 5);
    REQUIRE(root["total"].asUInt64() == 5);
    REQUIRE(root["offset"].asUInt64() == 0);
    REQUIRE(root["limit"].asUInt64() == 1000);
    REQUIRE_FALSE(root["hasMore"].asBool());

    const auto& first =
        root["objects"][Json::ArrayIndex{0}];

    REQUIRE(
        first["code"].asString()
        == "BUILDING_ADMIN"
    );

    REQUIRE(
        first["parentId"].asString()
        == "site-main"
    );

    REQUIRE_FALSE(
        first["isRoot"].asBool()
    );
}

TEST_CASE(
    "Objects endpoint applies filters and pagination",
    "[scada_http][objects][query]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto filtered =
        dispatcher.dispatch(
            make_request(
                "/api/objects",
                "parentId=site-main"
            )
        );

    const auto filtered_root =
        parse_json(filtered.body);

    REQUIRE(
        filtered_root["total"].asUInt64()
        == 1
    );

    REQUIRE(
        filtered_root["objects"]
            [Json::ArrayIndex{0}]
            ["id"]
            .asString()
        == "building-admin"
    );

    const auto paginated =
        dispatcher.dispatch(
            make_request(
                "/api/objects",
                "limit=2&offset=1"
            )
        );

    const auto paginated_root =
        parse_json(paginated.body);

    REQUIRE(
        paginated_root["count"].asUInt64()
        == 2
    );

    REQUIRE(
        paginated_root["total"].asUInt64()
        == 5
    );

    REQUIRE(
        paginated_root["offset"].asUInt64()
        == 1
    );

    REQUIRE(
        paginated_root["limit"].asUInt64()
        == 2
    );

    REQUIRE(
        paginated_root["hasMore"].asBool()
    );

    REQUIRE(
        paginated_root["objects"]
            [Json::ArrayIndex{0}]
            ["code"]
            .asString()
        == "CONTROL_ROOM"
    );
}

TEST_CASE(
    "Objects endpoint includes descendants when requested",
    "[scada_http][objects][query]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "/api/objects",
                "id=site-main&includeChildren=true"
            )
        );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["total"].asUInt64() == 3);
    REQUIRE(root["objects"].size() == 3);
}

TEST_CASE(
    "Object tree endpoint returns nested hierarchy",
    "[scada_http][objects][tree]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "/api/objects/tree"
            )
        );

    REQUIRE(
        response.status
        == HttpStatusCode::Ok
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["roots"].isArray());
    REQUIRE(root["roots"].size() == 2);
    REQUIRE(root["count"].asUInt64() == 5);

    const auto& site =
        root["roots"][Json::ArrayIndex{0}];

    REQUIRE(
        site["id"].asString()
        == "site-main"
    );

    REQUIRE(site["parentId"].isNull());

    REQUIRE(
        site["children"].size()
        == 1
    );

    REQUIRE(
        site["children"]
            [Json::ArrayIndex{0}]
            ["id"]
            .asString()
        == "building-admin"
    );

    REQUIRE(
        site["children"]
            [Json::ArrayIndex{0}]
            ["children"]
            [Json::ArrayIndex{0}]
            ["id"]
            .asString()
        == "room-control"
    );
}

TEST_CASE(
    "Object tree endpoint returns selected subtree",
    "[scada_http][objects][tree][query]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "/api/objects/tree",
                "id=site-main"
            )
        );

    const auto root =
        parse_json(response.body);

    REQUIRE(root["roots"].size() == 1);
    REQUIRE(root["count"].asUInt64() == 3);

    REQUIRE(
        root["roots"]
            [Json::ArrayIndex{0}]
            ["id"]
            .asString()
        == "site-main"
    );
}

TEST_CASE(
    "Objects endpoint rejects invalid limit",
    "[scada_http][objects][error]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "/api/objects",
                "limit=0"
            )
        );

    REQUIRE(
        response.status
        == HttpStatusCode::BadRequest
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(
        root["error"]["code"].asString()
        == "invalid_query"
    );

    REQUIRE(
        root["error"]["correlationId"].asString()
        == "object-request-110"
    );

    REQUIRE(
        root["error"]["details"]["field"].asString()
        == "limit"
    );

    REQUIRE(
        root["error"]["details"]["reason"].asString()
        == "must_be_positive"
    );
}

TEST_CASE(
    "Object endpoints reject unsupported query parameter",
    "[scada_http][objects][error]"
)
{
    const auto dispatcher =
        make_dispatcher();

    const auto response =
        dispatcher.dispatch(
            make_request(
                "/api/objects/tree",
                "unexpected=value"
            )
        );

    REQUIRE(
        response.status
        == HttpStatusCode::BadRequest
    );

    const auto root =
        parse_json(response.body);

    REQUIRE(
        root["error"]["details"]["field"].asString()
        == "unexpected"
    );

    REQUIRE(
        root["error"]["details"]["reason"].asString()
        == "unsupported_parameter"
    );
}
