add_library(execution)

target_sources(execution
    PRIVATE
        src/temp.cpp
)

target_include_directories(execution
    PUBLIC
        include
)

target_link_libraries(execution
    PUBLIC
        common
)