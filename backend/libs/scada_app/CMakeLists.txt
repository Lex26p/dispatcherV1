add_library(scada_app STATIC
    src/app_module.cpp
    src/application_composition.cpp
    src/development_configuration_read_service.cpp
    src/gateway_startup.cpp
)

target_include_directories(scada_app
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_app
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_app
    PUBLIC
        scada_common
        scada_core
        scada_api
        scada_realtime
    PRIVATE
        scada_objects
        scada_devices
        scada_tags
)
