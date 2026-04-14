add_executable(basic_inference
    src/inference.cpp
)

target_link_libraries(basic_inference
    PRIVATE
        sdk
)