add_library(scada_api STATIC
    src/api_endpoint.cpp
    src/api_mapper.cpp
    src/api_module.cpp
    src/api_read_endpoint.cpp
    src/api_response.cpp
    src/api_route_registry.cpp
    src/configuration_read_models.cpp
)

target_include_directories(scada_api
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_api
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_api
    PUBLIC
        scada_common
        scada_core
        scada_contracts
        scada_objects
        scada_devices
        scada_tags
)
