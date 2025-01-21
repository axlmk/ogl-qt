#include "RenderingWindow.hpp"

RenderingWindow::RenderingWindow() : m_KeyBeingPressed{false} {
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

	m_deltaTime = QDateTime::currentMSecsSinceEpoch() - m_currentTime;
	m_currentTime = QDateTime::currentMSecsSinceEpoch();


	g_opengl.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	float speed = 2 * m_deltaTime * 0.001;
	auto camPos = m_cam->getPosition();
	if(m_KeyBeingPressed[0])
		camPos.z -= speed;
	if (m_KeyBeingPressed[1])
		camPos.x -= speed;
	if (m_KeyBeingPressed[2])
		camPos.x += speed;
	if (m_KeyBeingPressed[3])
		camPos.z += speed;
	m_cam->setPosition(camPos);



	static unsigned int i = 0;
	for (auto &renderable : m_toRender) {
		auto pos = m_cam->getPosition();
		renderable->render();
		i++;
	}
}

void RenderingWindow::keyPressEvent(QKeyEvent* event) {
	if(event->text() == "z")
		m_KeyBeingPressed[0] = true;

	if(event->text() == "q")
		m_KeyBeingPressed[1] = true;

	if (event->text() == "d")
		m_KeyBeingPressed[2] = true;

	if (event->text() == "s")
		m_KeyBeingPressed[3] = true;
}

	void RenderingWindow::keyReleaseEvent(QKeyEvent * event) {
	if (event->text() == "z")
		m_KeyBeingPressed[0] = false;

	if (event->text() == "q")
		m_KeyBeingPressed[1] = false;

	if (event->text() == "d")
		m_KeyBeingPressed[2] = false;

	if (event->text() == "s")
		m_KeyBeingPressed[3] = false;
}