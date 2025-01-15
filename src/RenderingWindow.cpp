#include "RenderingWindow.hpp"

RenderingWindow::RenderingWindow() {
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, QOverload<>::of(&RenderingWindow::update));
	m_timer->start(16);
}



RenderingWindow::~RenderingWindow() {

}

void RenderingWindow::initializeGL() {

	/* Initialize OpenGL */
	g_opengl.initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);

	auto mainCam{ std::make_shared<Camera>(SpaceCoord(0.f, 0.0f, -2.f)) };

	auto texturedCube{ std::make_shared<Geometry>(GeometryType::Cube, .5) };
	texturedCube->setPivot(.25, .25, -.25);
	m_geometries.push_back(texturedCube);

	auto textShader{ std::make_shared<Shader>(ShaderType::Texture) };
	textShader->setTexture("textures/container.jpg");
	m_shaders.push_back(textShader);
	
	m_toRender.push_back(std::make_unique<Renderable>(m_geometries[0], m_shaders[0], mainCam));
}

void RenderingWindow::paintGL() {

	g_opengl.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static unsigned int i = 0;
	for (auto &renderable : m_toRender) {
		renderable->getGeometry()->rotate((i++) * 0.02, 0., 1.);
		renderable->render();
	}
}
