add_library(common)

target_sources(common
    PRIVATE
        src/temp.cpp
)

target_include_directories(common
    PUBLIC
        include
)

