#include "SceneViewer.hpp"
#include "SceneManager.hpp"


SceneViewer::SceneViewer(SceneManager* sceneManager) :
	m_manager{ sceneManager },
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

	m_manager->initializeScene();
}

void SceneViewer::paintGL() {

	m_deltaTime = QDateTime::currentMSecsSinceEpoch() - m_currentTime;
	m_currentTime = QDateTime::currentMSecsSinceEpoch();
	
	g_opengl.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static int i = 0;

	for (auto &sceneObject : m_manager->getSceneObjects()) {
		m_manager->getCamera()->walk(m_KeyBeingPressed, m_deltaTime);
		m_manager->getGeometries()[0]->rotate(i * 0.01, 1, 0, 0);
		m_manager->getGeometries()[1]->rotate(i * 0.01, 1, 0, 0);
		sceneObject->render(m_manager->getCamera().get());
	}
	i++;
}

void SceneViewer::keyPressEvent(QKeyEvent* event) {
	
	if(event->text() == "z")
		m_KeyBeingPressed[0] = true;

	if(event->text() == "q")
		m_KeyBeingPressed[1] = true;

	if(event->text() == "d")
		m_KeyBeingPressed[2] = true;

	if(event->text() == "s")
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
	// AGA DBG
	return;
	glm::vec2 delta{};
	float intensity = .05f;

	auto center = geometry().center();
	auto globalCenter = mapToGlobal(center);

	delta.x = (event->pos().x() - center.x()) * intensity;
	delta.y = (center.y() - event->pos().y()) * intensity;

	m_manager->getCamera()->addRotation(delta.x, delta.y);
	cursor().setPos(globalCenter.x(), globalCenter.y());
}