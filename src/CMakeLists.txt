# ---- Helper function to avoid repetition ----
function(setup_qt_target target)
    target_link_libraries(${target} PRIVATE ${COMMON_LIBS})
    target_include_directories(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../tests
    )

    if (MSVC)
        target_compile_options(${target} PUBLIC /W4 /permissive- /Zc:__cplusplus)
    else()
        target_compile_options(${target} PUBLIC -Wall -Wextra -Wpedantic)
    endif()

    # Copy resources
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${target}>/resources
    )

    if (WIN32)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${target}>
                $<TARGET_FILE_DIR:${target}>
            COMMAND_EXPAND_LISTS
        )

        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND Qt6::windeployqt
                --no-compiler-runtime
                $<TARGET_FILE:${target}>
        )
    endif()
endfunction()

# ---- Main executable ----
qt_add_executable(ogl-qt
    main.cpp
    App3D.cpp App3D.hpp
    App3DViewer.cpp App3DViewer.hpp
    Camera.cpp Camera.hpp
    Gizmo.cpp Gizmo.hpp
    HUD.cpp HUD.hpp
    LightObject.cpp LightObject.hpp
    LightProperties.cpp LightProperties.hpp
    Mesh.cpp Mesh.hpp
    Model.cpp Model.hpp
    PickingTexture.cpp
    Utils.hpp
    Scene.cpp Scene.hpp
    SceneObject.cpp SceneObject.hpp
    SceneViewer.cpp SceneViewer.hpp
    Shader.cpp Shader.hpp
)

setup_qt_target(ogl-qt)

set_target_properties(ogl-qt PROPERTIES MACOSX_BUNDLE ON)

# ---- Test executable ----
qt_add_executable(runtime_test
    main_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests/smoke_test.cpp
)

setup_qt_target(runtime_test)

add_test(NAME RuntimeSmoke COMMAND $<TARGET_FILE:runtime_test>)