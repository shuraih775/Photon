add_executable(logistic_regression
    src/inference.cpp
    src/image_loader.cpp
)

include(FetchContent)

FetchContent_Declare(
    stb_image
    URL https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
    DOWNLOAD_NO_EXTRACT TRUE
)

FetchContent_MakeAvailable(stb_image)

add_library(stb_image INTERFACE)

target_include_directories(stb_image
    INTERFACE
        ${stb_image_SOURCE_DIR}
)

target_link_libraries(logistic_regression
    PRIVATE
        sdk
        stb_image
)