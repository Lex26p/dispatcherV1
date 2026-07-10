find_package(Drogon CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_library(scada_http STATIC
    src/http_module.cpp
    src/http_types.cpp
    src/http_query.cpp
    src/correlation_id.cpp
    src/json_value.cpp
    src/api_error.cpp
    src/http_router.cpp
    src/drogon_http_server.cpp
    src/system_health_endpoint.cpp
    src/system_modules_endpoint.cpp
    src/system_endpoint_registry.cpp
    src/object_read_endpoint.cpp
    src/device_read_endpoint.cpp
    src/tag_read_endpoint.cpp
)

target_include_directories(scada_http
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_http
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_http
    PUBLIC
        scada_common
        scada_core
        scada_api
    PRIVATE
        Drogon::Drogon
        JsonCpp::JsonCpp
        Threads::Threads
)
