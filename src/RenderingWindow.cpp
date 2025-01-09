#include "RenderingWindow.hpp"

RenderingWindow::~RenderingWindow() {

}

void RenderingWindow::initializeGL() {

	/* Initialize OpenGL */
	try {
		g_opengl.initializeOpenGLFunctions();
	} catch (...) {
		qCritical() << "OpenGL could not be initialized";
	}

	Location loc {-0.5f, -0.5f, 0.0f};
	m_geometries.push_back(std::make_shared<Geometry>(GeometryType::Square, loc, .5));
	m_shaders.push_back(std::make_shared<Shader>("shaders/uni_color.vs", "shaders/uni_color_orange.fs"));
	m_toRender.push_back(std::make_unique<Renderable>(m_geometries[0], m_shaders[0]));

	Location loc2{ 0.0f, 0.0f, 0.0f };
	m_geometries.push_back(std::make_shared<Geometry>(GeometryType::Square, loc2, .5));
	m_shaders.push_back(std::make_shared<Shader>("shaders/uni_color.vs", "shaders/uni_color_green.fs"));
	m_toRender.push_back(std::make_unique<Renderable>(m_geometries[1], m_shaders[1]));
}

void RenderingWindow::paintGL() {

	g_opengl.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT);

	for (auto &renderable : m_toRender) {
		renderable->render();
	}
}
