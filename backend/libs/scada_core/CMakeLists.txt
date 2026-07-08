add_library(scada_core STATIC
    src/module_info.cpp
)

target_include_directories(scada_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_core
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_core
    PUBLIC
        scada_common
)