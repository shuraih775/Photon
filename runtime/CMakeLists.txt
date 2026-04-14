add_library(runtime)

target_sources(runtime
    PRIVATE
        src/runtime_manager.cpp
        src/runtime_context.cpp
        src/execution_engine_selector.cpp
)

target_include_directories(runtime
    PUBLIC
        include
)

target_link_libraries(runtime
    PUBLIC
        common
        core
        backend_core
        onnx
)