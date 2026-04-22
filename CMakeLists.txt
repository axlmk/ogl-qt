cmake_minimum_required(VERSION 3.16)
project(ogl-qt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(ASSIMP_BUILD_TESTS OFF )
set(ASSIMP_NO_EXPORT OFF )
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT  OFF )
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT  OFF )
set(ASSIMP_INSTALL_PDB  OFF )
set(ASSIMP_BUILD_ASSIMP_VIEW  OFF )
set(ASSIMP_BUILD_OBJ_IMPORTER  ON )

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 8d1fd52e5ab5590e2c81768ace50c72bae28f2ed # 1.0.3
)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG e0b52347c6e52de2827ec957a9ebf00ce3c54f79 # 6.0.4
)
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG VER-2-13-2
)

FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(stb)
FetchContent_MakeAvailable(assimp)
FetchContent_MakeAvailable(freetype)

set(STB_IMAGE_IMPL_FILE ${CMAKE_CURRENT_BINARY_DIR}/stb_image.cpp)

file(WRITE ${STB_IMAGE_IMPL_FILE}
"#define STB_IMAGE_IMPLEMENTATION\n#define STB_IMAGE_WRITE_IMPLEMENTATION\n#include <stb_image.h>\n#include <stb_image_write.h>\n"
)

add_library(stb_image STATIC)
add_library(stb::image ALIAS stb_image)


target_sources(stb_image
    PRIVATE
        ${STB_IMAGE_IMPL_FILE}
)

target_include_directories(stb_image
    PUBLIC
        ${stb_SOURCE_DIR}
)

target_compile_features(stb_image
    PUBLIC
        cxx_std_20
)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL Gui OpenGLWidgets)

qt_standard_project_setup()

add_subdirectory(src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set_target_properties(ogl-qt PROPERTIES
    # WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)