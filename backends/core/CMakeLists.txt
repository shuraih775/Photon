add_library(backend_core INTERFACE)

target_include_directories(backend_core
    INTERFACE
        include
)

target_link_libraries(backend_core
    INTERFACE
        common
)