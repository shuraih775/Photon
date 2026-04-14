add_library(onnx)

target_sources(onnx
    PRIVATE
        src/onnx_backend.cpp
        src/onnx_model.cpp
        src/onnx_factory.cpp
)

target_include_directories(onnx
    PUBLIC
        include
)

# Try to find a local installation first
find_package(onnxruntime CONFIG QUIET)

# If not found locally, automatically download it
if(NOT onnxruntime_FOUND)
    include(FetchContent)
    
    # URL for Linux x64. Change this URL if you are on Windows or macOS.
    FetchContent_Declare(
        onnxruntime_download
        URL https://github.com/microsoft/onnxruntime/releases/download/v1.28.0/onnxruntime-linux-x64-1.28.0.tgz
        URL_HASH SHA256=a3e1b79d7bb1bf09696ce675f49e4064e6c81f6202b8225624fff0e93f8d6407
    )
    
    FetchContent_MakeAvailable(onnxruntime_download)
    
    # Set up the include path and binary path from the downloaded folder
    set(ORT_INCLUDE_DIR "${onnxruntime_download_SOURCE_DIR}/include")
    
    find_library(ORT_LIB 
        NAMES onnxruntime
        PATHS "${onnxruntime_download_SOURCE_DIR}/lib"
        REQUIRED
    )
    
    # Create an interface for the downloaded library
    add_library(onnxruntime::onnxruntime SHARED IMPORTED GLOBAL)
    set_target_properties(onnxruntime::onnxruntime PROPERTIES
        IMPORTED_LOCATION "${ORT_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${ORT_INCLUDE_DIR}"
    )
endif()

# 3. Link everything up safely
target_link_libraries(onnx
    PUBLIC
        common
        backend_core
    PRIVATE
        onnxruntime::onnxruntime
)
