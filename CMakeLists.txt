cmake_minimum_required(VERSION 3.16)

project("BignumSqrt" VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable")

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)
add_subdirectory(src)
add_subdirectory(test)
