cmake_minimum_required(VERSION 3.18)
project(gpu_img_pipeline LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs highgui)

add_executable(pipeline
    src/main.cpp
    src/image_utils.cpp
    src/cpu/grayscale_cpu.cpp
    src/gpu/grayscale.cu
)

set_target_properties(pipeline PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
)

target_include_directories(pipeline PRIVATE include)
target_link_libraries(pipeline PRIVATE ${OpenCV_LIBS})

message(STATUS "OpenCV ver: ${OpenCV_VERSION}")
