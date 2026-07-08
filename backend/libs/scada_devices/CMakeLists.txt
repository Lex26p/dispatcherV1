add_library(scada_devices STATIC
    src/device.cpp
    src/device_connection.cpp
    src/device_module.cpp
    src/device_protocol.cpp
    src/device_state.cpp
)

target_include_directories(scada_devices
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_devices
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_devices
    PUBLIC
        scada_common
        scada_core
        scada_objects
)