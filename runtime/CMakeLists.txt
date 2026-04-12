add_library(runtime)

target_sources(runtime
    PRIVATE
)

target_include_directories(runtime
    PUBLIC
        include
)

target_link_libraries(runtime
    PUBLIC
        common
)