qt_add_executable(ogl-qt
    main.cpp
    App3D.cpp
    App3D.hpp
    App3DViewer.cpp
    App3DViewer.hpp
    Camera.cpp
    Camera.hpp
    Gizmo.cpp
    HUD.cpp
    HUD.hpp
    LightObject.cpp
    LightObject.hpp
    LightProperties.cpp
    LightProperties.hpp
    Mesh.cpp
    Mesh.hpp
    Model.cpp
    Model.hpp
    PickingTexture.cpp
    Utils.hpp
    Scene.cpp
    Scene.hpp
    SceneObject.cpp
    SceneObject.hpp
    SceneViewer.cpp
    SceneViewer.hpp
    Shader.cpp
    Shader.hpp
)

target_link_libraries(ogl-qt
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        glm::glm
        freetype
        stb::image
        assimp
        opengl32)

target_include_directories(ogl-qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if (MSVC)
    target_compile_options(ogl-qt PUBLIC
        /W4
        /permissive-
        /Zc:__cplusplus
    )
else()
    target_compile_options(ogl-qt PUBLIC
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

if(WIN32)
    add_custom_command(TARGET ogl-qt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:ogl-qt>
            $<TARGET_FILE_DIR:ogl-qt>
        COMMAND_EXPAND_LISTS
    )

    add_custom_command(TARGET ogl-qt POST_BUILD
        COMMAND Qt6::windeployqt
            --no-compiler-runtime
            $<TARGET_FILE:ogl-qt>
    )
endif()

add_custom_command(TARGET ogl-qt POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:ogl-qt>/resources
)

# = = = = = = TESTING = = = = = =

qt_add_executable(runtime_test main_test.cpp ../tests/smoke_test.cpp)

target_link_libraries(runtime_test
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::OpenGL
        Qt6::OpenGLWidgets
        glm::glm
        freetype
        stb::image
        assimp
        opengl32)

target_include_directories(runtime_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../tests)

if(WIN32)
    add_custom_command(TARGET runtime_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:runtime_test>
            $<TARGET_FILE_DIR:runtime_test>
        COMMAND_EXPAND_LISTS
    )

    add_custom_command(TARGET runtime_test POST_BUILD
        COMMAND Qt6::windeployqt
            --no-compiler-runtime
            $<TARGET_FILE:runtime_test>
    )
endif()

add_custom_command(TARGET runtime_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:runtime_test>/resources
)

add_test(NAME RuntimeSmoke COMMAND runtime_test)