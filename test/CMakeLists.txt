cmake_minimum_required(VERSION 3.16)

find_package(GTest REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(
    ${OPENSSL_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    )

link_libraries(
    ${CMAKE_PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES}
    pthread )

add_executable (test
    test.cpp
)

