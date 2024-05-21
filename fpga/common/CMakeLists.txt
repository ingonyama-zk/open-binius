cmake_minimum_required(VERSION 3.10)

project(common)

include_directories(/tools/Xilinx/Vitis_HLS/2023.2/include/)
include_directories(include/)

add_executable(
        test_binary_tower
        test/test_binary_tower.cpp
)
