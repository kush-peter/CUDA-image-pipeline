cmake_minimum_required(VERSION 3.18)
project(gpu_img_pipeline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

add_executable(pipeline
    src/main.cpp
    src/image_utils.cpp    
)

target_include_directories(pipeline PRIVATE include)
target_link_libraries(pipeline PRIVATE ${OpenCV_LIBS})
message(STATUS "OpenCV ver: ${OpenCV_VERSION}")