add_executable(dispatcher_server
    src/main.cpp
)

target_compile_features(dispatcher_server
    PRIVATE
        cxx_std_20
)

set_target_properties(dispatcher_server
    PROPERTIES
        OUTPUT_NAME "dispatcher_server"
)