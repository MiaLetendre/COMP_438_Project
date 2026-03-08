cmake_minimum_required(VERSION 3.16)
project(vineGen_cpp)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Python_EXECUTABLE "C:/Users/miale/AppData/Local/Programs/Python/Python311/python.exe")
set(Python_INCLUDE_DIR "C:/Users/miale/AppData/Local/Programs/Python/Python311/include")
set(Python_LIBRARY "C:/Users/miale/AppData/Local/Programs/Python/Python311/libs/python311.lib")
set(PYBIND11_FINDPYTHON ON)


include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1 # Or latest version
)
FetchContent_MakeAvailable(pybind11)
# Set libigl options before fetching

FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG        3.4.0 # Or latest version
)
FetchContent_MakeAvailable(Eigen)


add_executable(vine_debug 
    src/vineGen.cpp
    src/node.cpp
    src/Turt.cpp
    src/main.cpp
)

#delete these two parts after testing
pybind11_add_module(vineGen_cpp
    src/vineGen.cpp
    src/node.cpp
    src/Turt.cpp
    src/main.cpp
)
target_link_libraries(vine_debug PRIVATE 
    Eigen3::Eigen
    pybind11::module
)


target_link_libraries(vineGen_cpp PRIVATE 
    Eigen3::Eigen
)
#target_link_directories(vineGen_cpp PRIVATE ${EIGEN_SOURCE_DIR}/build/lib)

set_target_properties(vineGen_cpp PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/vineGen"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/vineGen"
    PREFIX ""
    SUFFIX ".pyd"
)



#tetst 