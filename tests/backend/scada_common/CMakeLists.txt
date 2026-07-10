add_executable(scada_common_tests
    version_tests.cpp
)

target_compile_features(scada_common_tests
    PRIVATE
        cxx_std_20
)

target_link_libraries(scada_common_tests
    PRIVATE
        scada_common
        Catch2::Catch2WithMain
)

catch_discover_tests(scada_common_tests
    TEST_PREFIX "scada_common."
)
