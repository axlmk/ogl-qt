#pragma once

#include <iostream>
#include <string>
#include <cmath>

#include <QOpenGLExtraFunctions>
#include <QOpenGLWindow>
#include <QDebug>
#include <QRandomGenerator>

#include "Utils.hpp"
#include "Shader.hpp"

class RectangleWindow : public QOpenGLWindow {
public:
	~RectangleWindow() Q_DECL_OVERRIDE;

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

private:
	unsigned int m_shaderProgramId;
	unsigned int m_VAO;
	Shader m_shd;
	std::string getFileContent(std::filesystem::path path);
};