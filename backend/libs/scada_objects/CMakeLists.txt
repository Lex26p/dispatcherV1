add_library(scada_objects STATIC
    src/object_module.cpp
    src/object_node.cpp
    src/object_path.cpp
    src/object_tree.cpp
    src/object_type.cpp
)

target_include_directories(scada_objects
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_objects
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_objects
    PUBLIC
        scada_common
        scada_core
)