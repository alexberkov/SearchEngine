cmake_minimum_required(VERSION 3.20)
project(SearchEngineApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/alexberkov/Qt/5.15.2/clang_64/lib/cmake")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(SearchEngineApp QtApp.cpp ../SearchServer.cpp ../ConverterJson.cpp ../InvertedIndex.cpp)
target_link_libraries(SearchEngineApp
        nlohmann_json::nlohmann_json
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )