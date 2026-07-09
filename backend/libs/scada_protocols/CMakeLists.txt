add_library(scada_protocols STATIC
    src/protocol_capabilities.cpp
    src/protocol_module.cpp
    src/protocol_requests.cpp
    src/protocol_results.cpp
)

target_include_directories(scada_protocols
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_protocols
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_protocols
    PUBLIC
        scada_common
        scada_core
        scada_devices
        scada_tags
)