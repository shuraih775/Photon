add_library(sdk)

target_sources(sdk
    PRIVATE
)

target_include_directories(sdk
    PUBLIC
        include
)

target_link_libraries(sdk
    PUBLIC
        common
)