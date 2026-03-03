add_executable(ogl-qt
main.cpp
App3D.cpp
App3D.hpp
App3DViewer.cpp
App3DViewer.hpp
)

target_include_directories(ogl-qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ogl-qt PRIVATE Qt6::Widgets)

# Camera.cpp
# Geometry.cpp
# Gizmo.cpp
# HUD.cpp
# Mesh.cpp
# Model.cpp
# PickingTexture.cpp
# Scene.cpp
# SceneManager.cpp
# SceneObject.cpp
# SceneViewer.cpp
# Shader.cpp
# Utils.cpp
# Vertex.cpp