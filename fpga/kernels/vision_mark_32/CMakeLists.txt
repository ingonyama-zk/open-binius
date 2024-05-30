cmake_minimum_required(VERSION 3.10)

project(vision_mark_32)

include_directories(/tools/Xilinx/Vitis_HLS/2023.2/include/)
include_directories(../../common/include/)
include_directories(include/)

add_executable(
        test_vision_mark_32
        src/vision_mark_32.cpp
        test/test.cpp
)
