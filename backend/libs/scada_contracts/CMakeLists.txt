add_library(scada_contracts STATIC
    src/api_envelope.cpp
    src/device_dto.cpp
    src/object_dto.cpp
    src/polling_dto.cpp
    src/runtime_dto.cpp
    src/tag_dto.cpp
)

target_include_directories(scada_contracts
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(scada_contracts
    PUBLIC
        cxx_std_20
)

target_link_libraries(scada_contracts
    PUBLIC
        scada_common
)