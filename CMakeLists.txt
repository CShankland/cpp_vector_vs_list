cmake_minimum_required(VERSION 3.0)

project (VectorVsList)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(SOURCE main.cpp main.cpp)

add_executable(VectorVsList ${SOURCE})

install(TARGETS VectorVsList RUNTIME DESTINATION bin)
