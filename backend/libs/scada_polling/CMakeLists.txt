add_library(scada_polling STATIC
    src/polling_execution.cpp
    src/polling_group.cpp
    src/polling_group_mode.cpp
    src/polling_module.cpp
    src/polling_scheduler.cpp
    src/polling_task.cpp
    src/polling_task_state.cpp
)

target_include_directories(scada_polling
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_polling
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_polling
    PUBLIC
        scada_common
        scada_core
        scada_devices
        scada_tags
        scada_protocols
)
