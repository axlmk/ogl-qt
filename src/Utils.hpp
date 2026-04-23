#pragma once

#include <QDir>
#include <QOpenGLExtraFunctions>

#define INFO_LOG_SIZE 512

inline char g_infoLog[INFO_LOG_SIZE];
inline QOpenGLExtraFunctions g_opengl;
inline QDir appDir;