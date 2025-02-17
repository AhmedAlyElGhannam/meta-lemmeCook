# set minimum required version of cmake to avoid errors
cmake_minimum_required(VERSION 3.22)

# to make sure CMake knows that the target will not be executed on this machine
set(CMAKE_SYSTEM_NAME Generic)

# project name 
project(hellowarudo)

# define used languages
enable_language(C)

# src file name
set(SRC_FILE "main.c")

# compile and produce an executable
add_executable(${PROJECT_NAME} ${SRC_FILE}) 
