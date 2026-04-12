add_library(compute)

target_sources(compute
    PRIVATE
        src/temp.cpp
)

target_include_directories(compute
    PUBLIC
        include
)

target_link_libraries(compute
    PUBLIC
        common
)