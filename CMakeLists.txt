cmake_minimum_required(VERSION 3.10)

# set the project name
project(qalamc)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(qalamc qalam.cpp ir.cpp lexer.cpp qalamFrontend.cpp qiskitBackend.cpp quirkFrontend.cpp)