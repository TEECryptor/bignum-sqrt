cmake_minimum_required(VERSION 3.10)

project("bignum-sqrt" VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable")

cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0079 NEW)

find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(${CMAKE_PROJECT_NAME} SHARED 
        bignum-sqrt.cpp
        )

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${OPENSSL_INCLUDE_DIR}
        )

#target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE /usr/local/lib)
target_link_libraries(${CMAKE_PROJECT_NAME}
        OpenSSL::Crypto
        -ldl
        )