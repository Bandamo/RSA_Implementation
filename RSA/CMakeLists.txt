cmake_minimum_required(VERSION 3.15)
project(RSA_Implementation)

set(CMAKE_CXX_STANDARD 17)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${INC})

add_executable(RSA_Implementation ${SRC}/main.cpp ${SRC}/RSA.cpp ${INC}/RSA.h ${SRC}/uintn.cpp ${INC}/uintn.h)