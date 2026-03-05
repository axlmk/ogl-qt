cmake_minimum_required(VERSION 3.16)
project(ogl-qt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	8d1fd52e5ab5590e2c81768ace50c72bae28f2ed # 1.0.3
)

FetchContent_MakeAvailable(glm)


find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL Gui)

qt_standard_project_setup()

add_subdirectory(src)

set_target_properties(ogl-qt PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)