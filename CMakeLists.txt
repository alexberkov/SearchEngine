cmake_minimum_required(VERSION 3.22)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(SearchEngine ConverterJson.h App.h InvertedIndex.h SearchServer.h main.cpp Tests.cpp)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json)

FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

FetchContent_MakeAvailable(googletest)
enable_testing()

target_link_libraries(SearchEngine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(SearchEngine)