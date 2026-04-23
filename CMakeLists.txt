cmake_minimum_required(VERSION 3.16)
project(ogl-qt VERSION 1.0.0 LANGUAGES CXX)

# ---- Global settings ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Dependencies ----
include(FetchContent)

FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 8d1fd52e5ab5590e2c81768ace50c72bae28f2ed
)

FetchContent_Declare(stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG e0b52347c6e52de2827ec957a9ebf00ce3c54f79
)

FetchContent_Declare(freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG VER-2-13-2
)

# Assimp options
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)

FetchContent_MakeAvailable(glm stb assimp freetype)

# stb_image wrapper
set(STB_IMAGE_IMPL_FILE ${CMAKE_CURRENT_BINARY_DIR}/stb_image.cpp)

file(WRITE ${STB_IMAGE_IMPL_FILE}
"#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>"
)

add_library(stb_image STATIC ${STB_IMAGE_IMPL_FILE})
add_library(stb::image ALIAS stb_image)

target_include_directories(stb_image PUBLIC ${stb_SOURCE_DIR})
target_compile_features(stb_image PUBLIC cxx_std_20)

# ---- Qt ----
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core Gui Widgets OpenGL OpenGLWidgets
)

# ---- Shared libraries list ----
set(COMMON_LIBS
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    glm::glm
    freetype
    stb::image
    assimp
    opengl32
)

enable_testing()
add_subdirectory(src)