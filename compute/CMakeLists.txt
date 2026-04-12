add_library(compute)

target_sources(compute
    PRIVATE
)

target_include_directories(compute
    PUBLIC
        include
)

target_link_libraries(compute
    PUBLIC
        common
)