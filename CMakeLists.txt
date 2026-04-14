cmake_minimum_required(VERSION 3.24)

project(
    InferencePlatform
    VERSION 0.1.0
    DESCRIPTION "Modular AI Inference Runtime Platform"
    LANGUAGES CXX
)

# C++ Standard

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build Options

option(INFERENCE_BUILD_TESTS           "Build unit tests"               ON)
option(INFERENCE_BUILD_BENCHMARKS      "Build benchmarks"               OFF)
option(INFERENCE_BUILD_EXAMPLES        "Build examples"                 ON)
option(INFERENCE_BUILD_APPLICATIONS    "Build applications"             ON)

option(INFERENCE_ENABLE_SANITIZERS     "Enable sanitizers"              OFF)
option(INFERENCE_ENABLE_LTO            "Enable link time optimization"  OFF)
option(INFERENCE_ENABLE_WERROR         "Treat warnings as errors"       OFF)

# Output Directories

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# CMake Modules

list(APPEND CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/cmake"
)

include(ProjectOptions)
include(Warnings)
include(Dependencies)

# Subdirectories

add_subdirectory(common)
add_subdirectory(memory)
add_subdirectory(compute)
add_subdirectory(core)
add_subdirectory(backends)
add_subdirectory(execution)
add_subdirectory(scheduler)
add_subdirectory(runtime)
add_subdirectory(telemetry)
add_subdirectory(sdk)

if(INFERENCE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(INFERENCE_BUILD_APPLICATIONS)
    add_subdirectory(applications)
endif()

# Testing

if(INFERENCE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Benchmarks

if(INFERENCE_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()