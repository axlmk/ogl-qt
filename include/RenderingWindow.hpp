#pragma once

#include <iostream>
#include <string>
#include <cmath>

#include <QOpenGLExtraFunctions>
#include <QOpenGLWindow>
#include <QDebug>
#include <QRandomGenerator>
#include <QTimer>

#include "Utils.hpp"
#include "Shader.hpp"
#include "Geometry.hpp"
#include "Renderable.hpp"
#include "Camera.hpp"

class RenderingWindow : public QOpenGLWindow {
public:
	RenderingWindow();
	~RenderingWindow() Q_DECL_OVERRIDE;

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

private:
	std::vector<std::shared_ptr<Geometry>> m_geometries;
	std::vector<std::shared_ptr<Shader>> m_shaders;
	std::vector<std::unique_ptr<Renderable>> m_toRender;
	std::shared_ptr<Camera> m_cam;

	QTimer *m_timer;
};