<h1>
  <img width="90" align="center" alt="app_icon" src="https://github.com/axlmk/ogl-qt/raw/release_1.0/resources/icons/app_icon.png" />
  OGL-QT
</h1>

[![Current milestone completion](https://img.shields.io/github/milestones/progress/axlmk/ogl-qt/2)](https://github.com/axlmk/ogl-qt/milestone/2)

[![windows-latest-msvc](https://github.com/axlmk/ogl-qt/actions/workflows/windows-latest-msvc.yml/badge.svg)](https://github.com/axlmk/ogl-qt/actions/workflows/windows-latest-msvc.yml) [![windows-latest-mingw](https://github.com/axlmk/ogl-qt/actions/workflows/windows-latest-mingw.yml/badge.svg)](https://github.com/axlmk/ogl-qt/actions/workflows/windows-latest-mingw.yml) [![linux-latest-gcc](https://github.com/axlmk/ogl-qt/actions/workflows/linux-latest-gcc.yml/badge.svg)](https://github.com/axlmk/ogl-qt/actions/workflows/linux-latest-gcc.yml)

## Overview

A project to learn how to do graphics programming with OpenGL and Qt. It's also a playground to experiment with Github actions, Doxygen, Github project and CMake.

<img width="600" height="412" alt="presentation" src="https://github.com/user-attachments/assets/7b6e9266-8860-40b7-9d7a-17d7b270c48e" />

*Global overview of the project*

This project features:
 - 3D rendering .obj files with custom textures
 - Lighting with three different light casters
 - Selection and translation of any object loaded up into the scene
 - Maya like navigation and camera
 - Details panel to configure the different objects

But also:
 - Issues and milestones to help managing the project
 - Building pipeline on different OS / compilers
 - Doxygen documentation for the source code
 - A flexible building and dependency management system with CMake

## Building the project
**Ogl-Qt** uses CMake for the management of the dependencies and for building the entire project. Some presets are available under the `CMakePreset.json`. For more details, see the [documentation](https://github.com/axlmk/ogl-qt/wiki/Building-the-project).

## API Documentation
The entire codebase is documented with doxygen and the full API documentation can be found [here](https://axlmk.github.io/ogl-qt/).
