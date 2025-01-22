#pragma once

#include <string>

#include <QDebug>
#include <QOpenGLExtraFunctions>

#define INFO_LOG_SIZE 512

extern char g_infoLog[INFO_LOG_SIZE];
extern QOpenGLExtraFunctions g_opengl;
extern int g_deltaTime;