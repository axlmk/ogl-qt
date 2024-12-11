#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

#include <QOpenGLExtraFunctions>
#include <QOpenGLWindow>
#include <QDebug>

#define INFO_LOG_SIZE 512

class RectangleWindow : public QOpenGLWindow, protected QOpenGLExtraFunctions {
public:
	~RectangleWindow() Q_DECL_OVERRIDE;

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

private:
	unsigned int m_shaderProgramId;
	unsigned int m_VAO;
	std::string getFileContent(std::filesystem::path path);
};