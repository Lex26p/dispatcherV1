add_library(scada_realtime STATIC
    src/realtime_message.cpp
    src/realtime_module.cpp
    src/realtime_subscription.cpp
)

target_include_directories(scada_realtime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_realtime
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_realtime
    PUBLIC
        scada_common
        scada_core
)
