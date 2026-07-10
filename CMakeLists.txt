cmake_minimum_required(VERSION 3.20)

project(
    Dispatcher
    VERSION 0.1.0
    DESCRIPTION "Dispatcher SCADA and dispatching platform"
    LANGUAGES CXX
)

if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(
        CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>"
    )
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_subdirectory(backend/libs/scada_common)
add_subdirectory(backend/libs/scada_contracts)
add_subdirectory(backend/libs/scada_core)
add_subdirectory(backend/libs/scada_objects)
add_subdirectory(backend/libs/scada_devices)
add_subdirectory(backend/libs/scada_tags)
add_subdirectory(backend/libs/scada_protocols)
add_subdirectory(backend/libs/scada_polling)
add_subdirectory(backend/libs/scada_runtime)
add_subdirectory(backend/libs/scada_historian)
add_subdirectory(backend/libs/scada_events)
add_subdirectory(backend/libs/scada_alarms)
add_subdirectory(backend/libs/scada_api)
add_subdirectory(backend/libs/scada_http)
add_subdirectory(backend/libs/scada_realtime)
add_subdirectory(backend/libs/scada_app)
add_subdirectory(backend/apps/dispatcher_server)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
