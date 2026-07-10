add_library(scada_runtime STATIC
    src/engineering_transform.cpp
    src/runtime_event.cpp
    src/runtime_module.cpp
    src/runtime_quality.cpp
    src/runtime_value_applier.cpp
    src/runtime_value_change.cpp
    src/runtime_value_conversion.cpp
    src/tag_value_store.cpp
)

target_include_directories(scada_runtime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_runtime
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_runtime
    PUBLIC
        scada_common
        scada_core
        scada_tags
        scada_protocols
)
