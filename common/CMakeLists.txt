add_library(common)

target_sources(common
    PRIVATE
)

target_include_directories(common
    PUBLIC
        include
)

target_link_libraries(common
    PUBLIC
        common
)