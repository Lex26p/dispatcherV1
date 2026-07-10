add_library(scada_events STATIC
    src/event_module.cpp
    src/event_record.cpp
)

target_include_directories(scada_events
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_events
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_events
    PUBLIC
        scada_common
        scada_core
)
