add_library(scada_common STATIC
    src/error.cpp
    src/id_types.cpp
    src/timestamp.cpp
    src/version.cpp
)

target_include_directories(scada_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_common
    PUBLIC
        cxx_std_20
)