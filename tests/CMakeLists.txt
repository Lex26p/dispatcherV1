find_package(Catch2 3 CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

include(Catch)

add_subdirectory(backend/scada_common)
add_subdirectory(backend/scada_api)
add_subdirectory(backend/scada_app)
add_subdirectory(backend/scada_http)
