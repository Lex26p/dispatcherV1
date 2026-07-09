add_library(scada_tags STATIC
    src/tag.cpp
    src/tag_address.cpp
    src/tag_archive_policy.cpp
    src/tag_current_value.cpp
    src/tag_module.cpp
    src/tag_quality.cpp
    src/tag_type.cpp
    src/tag_validation.cpp
    src/tag_value_type.cpp
)

target_include_directories(scada_tags
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_tags
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_tags
    PUBLIC
        scada_common
        scada_core
        scada_objects
        scada_devices
)