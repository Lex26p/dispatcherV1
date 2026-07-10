add_executable(scada_api_tests
    configuration_read_models_tests.cpp
)

target_compile_features(scada_api_tests
    PRIVATE
        cxx_std_20
)

target_link_libraries(scada_api_tests
    PRIVATE
        scada_api
        Catch2::Catch2WithMain
)

catch_discover_tests(scada_api_tests
    TEST_PREFIX "scada_api."
)
