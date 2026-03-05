add_executable(ogl-qt
    main.cpp
    App3D.cpp
    App3D.hpp
    App3DViewer.cpp
    App3DViewer.hpp
    Utils.hpp
    Scene.cpp
    Scene.hpp
    SceneViewer.cpp
    SceneViewer.hpp
)

target_include_directories(ogl-qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ogl-qt
    PRIVATE
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::Gui
        glm::glm)

# Camera.cpp
# Geometry.cpp
# Gizmo.cpp
# HUD.cpp
# Mesh.cpp
# Model.cpp
# PickingTexture.cpp
# SceneManager.cpp
# SceneObject.cpp
# Shader.cpp
# Vertex.cpp