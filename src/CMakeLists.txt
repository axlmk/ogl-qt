cmake_minimum_required(VERSION 3.16)

project(ogl-qt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(ogl-qt
    App3D.cpp
    App3DViewer.cpp
    Camera.cpp
    Geometry.cpp
    Gizmo.cpp
    HUD.cpp
    main.cpp
    Mesh.cpp
    Model.cpp
    PickingTexture.cpp
    Scene.cpp
    SceneManager.cpp
    SceneObject.cpp
    SceneViewer.cpp
    Shader.cpp
    Utils.cpp
    Vertex.cpp
)

target_link_libraries(ogl-qt PRIVATE Qt6::Widgets)

set_target_properties(ogl-qt PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)