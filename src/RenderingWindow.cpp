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

	auto mainCam{ std::make_shared<Camera>(SpaceCoord(0., 0., 2.)) };
	mainCam->setType(CameraType::LookAt);
	mainCam->setTarget(SpaceCoord(0., 0., 0.));
	m_cam = mainCam;

	auto texturedCube{ std::make_shared<Geometry>(GeometryType::Cube, .5) };
	texturedCube->setPivot(.75, .25, -.25);
	m_geometries.push_back(texturedCube);

	auto faceCube{ std::make_shared<Geometry>(GeometryType::Cube, .5) };
	faceCube->setPivot(.25, .25, -.25);
	m_geometries.push_back(faceCube);
	

	auto textShader{ std::make_shared<Shader>(ShaderType::Texture) };
	textShader->setTexture("textures/container.jpg");
	m_shaders.push_back(textShader);

	auto faceShader{ std::make_shared<Shader>(ShaderType::Texture) };
	faceShader->setTexture("textures/awesomeface.png");
	m_shaders.push_back(faceShader);
	
	m_toRender.push_back(std::make_unique<Renderable>(m_geometries[0], m_shaders[0], mainCam));
	m_toRender.push_back(std::make_unique<Renderable>(m_geometries[1], m_shaders[1], mainCam));
}

void RenderingWindow::paintGL() {

	g_opengl.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float radius = 3.0f;

	static unsigned int i = 0;
	for (auto &renderable : m_toRender) {
		float camX = std::sin(i * 0.01) * radius;
		float camZ = std::cos(i * 0.01) * radius;
		
		renderable->getGeometry()->rotate((i) * 0.02, 1.);
		auto pos = m_cam->getPosition();
		m_cam->setPosition(SpaceCoord(camX, pos.y, camZ));
		renderable->render();
		i++;
	}
}
