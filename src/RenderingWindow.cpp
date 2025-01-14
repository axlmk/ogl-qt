#include "RenderingWindow.hpp"

RenderingWindow::~RenderingWindow() {

}

void RenderingWindow::initializeGL() {

	/* Initialize OpenGL */
	g_opengl.initializeOpenGLFunctions();

	auto mainCam{ std::make_shared<Camera>(SpaceCoord(0.f, 0.0f, -2.f)) };

	auto textSquare{ std::make_shared<Geometry>(GeometryType::Square, .5) };
	textSquare->translate(-.25f, -.25f);
	m_geometries.push_back(textSquare);

	auto textShader{ std::make_shared<Shader>(ShaderType::Texture) };
	textShader->setTexture("textures/container.jpg");
	m_shaders.push_back(textShader);
	
	m_toRender.push_back(std::make_unique<Renderable>(m_geometries[0], m_shaders[0], mainCam));
}

void RenderingWindow::paintGL() {

	g_opengl.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT);

	for (auto &renderable : m_toRender) {
		renderable->render();
	}
}
