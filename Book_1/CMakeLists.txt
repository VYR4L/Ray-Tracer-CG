cmake_minimum_required(VERSION 3.10)
project(RayTracerCG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")

add_executable(main main.cpp)
