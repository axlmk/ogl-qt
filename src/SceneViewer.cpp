#include "SceneViewer.hpp"



SceneViewer::SceneViewer(std::shared_ptr<SceneManager> sceneManager) :
	m_sceneManager{ sceneManager },
	m_KeyBeingPressed{ false, false, false, false },
	m_currentTime{ QDateTime::currentMSecsSinceEpoch() },
	m_timer{ std::make_unique<QTimer>(this) },
	m_deltaTime{ 0 } {

	connect(&(*m_timer), &QTimer::timeout, this, QOverload<>::of(&SceneViewer::update));
	m_timer->start(16);
}



SceneViewer::~SceneViewer() {

}



void SceneViewer::initializeGL() {

	g_opengl.initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);

	m_sceneManager->compileShaders();
	m_sceneManager->loadSceneObjects();
}

void SceneViewer::paintGL() {

	m_deltaTime = QDateTime::currentMSecsSinceEpoch() - m_currentTime;
	m_currentTime = QDateTime::currentMSecsSinceEpoch();
	
	g_opengl.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto &sceneObject : m_sceneManager->getSceneObjects()) {
		m_sceneManager->getCamera()->walk(m_KeyBeingPressed, m_deltaTime);
		sceneObject->render(m_sceneManager->getCamera());
	}
}

void SceneViewer::keyPressEvent(QKeyEvent* event) {
	
	if (event->text() == "z")
		m_KeyBeingPressed[0] = true;

	if (event->text() == "q")
		m_KeyBeingPressed[1] = true;

	if (event->text() == "d")
		m_KeyBeingPressed[2] = true;

	if (event->text() == "s")
		m_KeyBeingPressed[3] = true;
}

void SceneViewer::keyReleaseEvent(QKeyEvent * event) {
	if (event->text() == "z")
		m_KeyBeingPressed[0] = false;

	if (event->text() == "q")
		m_KeyBeingPressed[1] = false;

	if (event->text() == "d")
		m_KeyBeingPressed[2] = false;

	if (event->text() == "s")
		m_KeyBeingPressed[3] = false;
}



void SceneViewer::mouseMoveEvent(QMouseEvent* event) {
	glm::vec2 delta;
	float intensity = .05f;

	auto center = geometry().center();
	auto globalCenter = mapToGlobal(center);

	delta.x = (event->pos().x() - center.x()) * intensity;
	delta.y = (center.y() - event->pos().y()) * intensity;

	m_sceneManager->getCamera()->addRotation(delta.x, delta.y);
	cursor().setPos(globalCenter.x(), globalCenter.y());
}