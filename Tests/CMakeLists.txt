cmake_minimum_required(VERSION 3.22)
project(SearchEngineTests)

set(CMAKE_CXX_STANDARD 20)

add_executable(SearchEngineTests Tests.cpp)

enable_testing()

target_link_libraries(SearchEngineTests PRIVATE gtest_main nlohmann_json::nlohmann_json)
include(GoogleTest)
gtest_discover_tests(SearchEngineTests)

