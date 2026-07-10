add_executable(scada_http_tests
    http_types_tests.cpp
    http_query_tests.cpp
    correlation_id_tests.cpp
    json_value_tests.cpp
    api_error_tests.cpp
    http_router_tests.cpp
    system_endpoint_tests.cpp
    object_read_endpoint_tests.cpp
    device_read_endpoint_tests.cpp
)

target_compile_features(scada_http_tests
    PRIVATE
        cxx_std_20
)

target_link_libraries(scada_http_tests
    PRIVATE
        scada_http
        scada_app
        JsonCpp::JsonCpp
        Catch2::Catch2WithMain
)

catch_discover_tests(scada_http_tests
    TEST_PREFIX "scada_http."
)
