add_library(scheduler)

target_sources(scheduler
    PRIVATE
)

target_include_directories(scheduler
    PUBLIC
        include
)

target_link_libraries(scheduler
    PUBLIC
        common
)