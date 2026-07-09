add_library(scada_alarms STATIC
    src/alarm_lifecycle.cpp
    src/alarm_module.cpp
    src/alarm_record.cpp
)

target_include_directories(scada_alarms
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_alarms
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_alarms
    PUBLIC
        scada_common
        scada_core
        scada_events
)