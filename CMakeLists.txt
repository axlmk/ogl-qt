cmake_minimum_required(VERSION 3.16)
project(ogl-qt VERSION 1.0.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_subdirectory(src)

set_target_properties(ogl-qt PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)