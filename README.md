# OGL-QT

## A project to learn the usage of OpenGL and Qt

## 1. Build the project 
### My prefered way to build the project
Using VSCode, I append the paths of the CMake executable and of the compiler in the `settings.json` file of the user.
```json
"cmake.cmakePath": "C:\\Qt\\Tools\\CMake_64\\bin\\cmake",
"cmake.environment": {
    "PATH": "C:\\Qt\\Tools\\mingw1310_64\\bin;${env:PATH}"
}
```
I then create a custom `CMakeUserPresets.json` file to specify the paths of the compiler, the Qt's dependencies and the build system.
```json
{
    "version" : 6,
    "configurePresets": [
        {
            "name": "win_debug_usr",
            "displayName": "Windows debug Axel",
            "inherits": "win_debug",
            "cacheVariables": {
                "CMAKE_MAKE_PROGRAM" : "C:\\Qt\\Tools\\Ninja\\ninja.exe",
                "CMAKE_PREFIX_PATH" : "C:\\Qt\\6.11.0\\mingw_64",
                "CMAKE_CXX_COMPILER" : "C:\\Qt\\Tools\\mingw1310_64\\bin\\g++.exe"
            }
        }
    ]
}
```