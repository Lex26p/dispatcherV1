add_library(scada_historian STATIC
    src/archive_decision.cpp
    src/historian_buffer.cpp
    src/historian_module.cpp
    src/history_batch.cpp
    src/history_query.cpp
    src/history_sample.cpp
)

target_include_directories(scada_historian
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_historian
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_historian
    PUBLIC
        scada_common
        scada_core
        scada_tags
)