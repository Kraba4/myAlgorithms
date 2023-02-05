cmake_minimum_required(VERSION 3.23)
project(myAlgorithms)

set(CMAKE_CXX_STANDARD 20)

add_executable(myAlgorithms ThreadPool.cpp ThreadPool.h main.cpp ModuleVector.cpp ModuleVector.h FindPool.h)
