cmake_minimum_required(VERSION 2.8)
project(test)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

find_package(OpenSSL REQUIRED)

add_executable(test main.cpp)
target_link_libraries(test OpenSSL::SSL)
