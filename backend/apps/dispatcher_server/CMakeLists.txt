add_executable(dispatcher_server
    src/main.cpp
)

target_compile_features(dispatcher_server
    PRIVATE
        cxx_std_20
)

target_link_libraries(dispatcher_server
    PRIVATE
        scada_common
        scada_contracts
        scada_core
        scada_objects
        scada_devices
        scada_tags
        scada_protocols
        scada_polling
        scada_runtime
        scada_historian
        scada_events
        scada_alarms
        scada_api
        scada_http
        scada_realtime
        scada_app
)

set_target_properties(dispatcher_server
    PROPERTIES
        OUTPUT_NAME "dispatcher_server"
)
