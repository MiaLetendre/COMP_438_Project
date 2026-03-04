cmake_minimum_required(VERSION 3.16)
project(vineGen_cpp)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python 3.11 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)
# Set libigl options before fetching

include(FetchContent)

pybind11_add_module(vineGen_cpp src/main.cpp)

target_include_directories(vineGen_cpp PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/vineGen"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/vineGen"
)

