add_library(scada_http STATIC
    src/http_module.cpp
    src/http_types.cpp
    src/http_router.cpp
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
)
