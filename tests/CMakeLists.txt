find_package(Catch2 3 CONFIG REQUIRED)

include(Catch)

add_subdirectory(backend/scada_common)
add_subdirectory(backend/scada_http)
