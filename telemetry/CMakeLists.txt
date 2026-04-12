add_library(telemetry)

target_sources(telemetry
    PRIVATE
)

target_include_directories(telemetry
    PUBLIC
        include
)

target_link_libraries(telemetry
    PUBLIC
        common
)