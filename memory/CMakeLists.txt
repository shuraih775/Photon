add_library(memory)

target_sources(memory
    PRIVATE
        src/temp.cpp
)

target_include_directories(memory
    PUBLIC
        include
)

target_link_libraries(memory
    PUBLIC
        common
)