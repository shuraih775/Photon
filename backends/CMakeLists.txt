add_subdirectory(core)

if(INFERENCE_ENABLE_ONNX)
    add_subdirectory(onnx)
endif()

if(INFERENCE_ENABLE_TENSORRT)
    add_subdirectory(tensorrt)
endif()