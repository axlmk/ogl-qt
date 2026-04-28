#pragma once

#include <QDir>
#include <QOpenGLExtraFunctions>

#define INFO_LOG_SIZE 512

inline char g_infoLog[INFO_LOG_SIZE];	///< The log string used by OpenGL
inline QOpenGLExtraFunctions g_opengl;	///< A global access to OpenGL functions
inline QDir appDir;						///< The directory of the application