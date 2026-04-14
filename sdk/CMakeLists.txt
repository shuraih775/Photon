add_library(sdk)

target_sources(sdk
    PRIVATE
        src/sdk.cpp
        src/runtime.cpp
        src/runtime_builder.cpp
)

target_include_directories(sdk
    PUBLIC
        include
)

target_link_libraries(sdk
    PUBLIC
        common
        core
        runtime
)