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
    App3D.cpp
    App3DViewer.cpp
    Camera.cpp
    Gizmo.cpp
    HUD.cpp
    LightObject.cpp
    LightProperties.cpp
    Mesh.cpp
    Model.cpp
    PickingTexture.cpp
    Utils.hpp
    Scene.cpp
    SceneObject.cpp
    SceneViewer.cpp
    Shader.cpp
)

setup_qt_target(ogl-qt)

set_target_properties(ogl-qt PROPERTIES
    MACOSX_BUNDLE ON
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:ogl-qt>"
)

# ---- Test executable ----
qt_add_executable(runtime_test
    main_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests/smoke_test.cpp
)

setup_qt_target(runtime_test)

add_test(NAME RuntimeSmoke COMMAND $<TARGET_FILE:runtime_test>)

set_tests_properties(RuntimeSmoke PROPERTIES WORKING_DIRECTORY $<TARGET_FILE_DIR:runtime_test>)