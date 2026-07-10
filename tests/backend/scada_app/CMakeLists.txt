add_executable(scada_app_tests
    development_configuration_read_service_tests.cpp
)

target_compile_features(scada_app_tests
    PRIVATE
        cxx_std_20
)

target_link_libraries(scada_app_tests
    PRIVATE
        scada_app
        scada_api
        Catch2::Catch2WithMain
)

catch_discover_tests(scada_app_tests
    TEST_PREFIX "scada_app."
)
