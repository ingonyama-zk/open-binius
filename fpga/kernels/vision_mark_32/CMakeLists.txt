cmake_minimum_required(VERSION 3.10)

project(vision_mark_32)

set(CMAKE_CXX_STANDARD 17)

include_directories(/tools/Xilinx/Vitis_HLS/2023.2/include/)

add_executable(
        vision_mark_32_test
        ./src/vision_mark_32.cpp
)