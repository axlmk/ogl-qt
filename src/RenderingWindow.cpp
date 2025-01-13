#include "RenderingWindow.hpp"

RenderingWindow::~RenderingWindow() {

}

void RenderingWindow::initializeGL() {

	/* Initialize OpenGL */
	g_opengl.initializeOpenGLFunctions();

	auto orangeSquare{ std::make_shared<Geometry>(GeometryType::Square, SpaceCoord(-0.5f, -0.5f, 0.f), .5) };
	m_geometries.push_back(orangeSquare);
	
	auto orangeShader { std::make_shared<Shader>(ShaderType::Unicolor) };
	orangeShader->setColor();
	m_shaders.push_back(orangeShader);

	auto textSquare{ std::make_shared<Geometry>(GeometryType::Square, SpaceCoord(0.0f, 0.0f, 0.0f), .5) };
	m_geometries.push_back(textSquare);

	auto textShader{ std::make_shared<Shader>(ShaderType::Texture) };
	textShader->setTexture("textures/container.jpg");
	m_shaders.push_back(textShader);
	
	m_toRender.push_back(std::make_unique<Renderable>(m_geometries[0], m_shaders[0]));
	m_toRender.push_back(std::make_unique<Renderable>(m_geometries[1], m_shaders[1]));
}

void RenderingWindow::paintGL() {

	g_opengl.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT);

	static unsigned int i = 0;
	for (auto &renderable : m_toRender) {
		renderable->render();
		if(i == 100) {
			renderable->linkShader(m_shaders[0]);
		}
		if(i == 150) {
			renderable->linkShader(m_shaders[1]);
		}
	}
	i++;
}
