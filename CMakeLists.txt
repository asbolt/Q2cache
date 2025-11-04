cmake_minimum_required(VERSION 3.10)
project(QCache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(qcache src/main.cpp)

add_executable(compare src/compare.cpp)

add_executable(test_cache test/test_cache.cpp)