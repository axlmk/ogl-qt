#include "SceneViewer.hpp"
#include "SceneManager.hpp"


SceneViewer::SceneViewer(SceneManager* sceneManager) :
	m_deltaTime			{ 0 },
	m_lastMousePos		{ 0.0, 0.0 },
	m_mousePosWhenPressed{ 0.0, 0.0 },
	m_altBeingPressed	{ false },
	m_mousePressed		{ false, false, false },
	m_KeyBeingPressed	{ false, false, false, false },
	m_manager			{ sceneManager },
	m_timer				{ std::make_unique<QTimer>(this) },
	m_currentTime		{ QDateTime::currentMSecsSinceEpoch() } {

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

	if(event->key() == Qt::Key_Alt ) {
		m_altBeingPressed = true;
	}
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

	if(event->key() == Qt::Key_Alt) {
		m_altBeingPressed = false;
	}
}

void SceneViewer::mousePressEvent(QMouseEvent* event) {
	switch (event->button()) {
		case Qt::LeftButton:
			m_mousePressed[0] = true;
			m_lastMousePos.x = event->pos().x();
			m_lastMousePos.y = event->pos().y();
			m_mousePosWhenPressed = m_lastMousePos;
			break;
		case Qt::MiddleButton:
			m_mousePressed[1] = true;
			break;
		case Qt::RightButton:
			m_mousePressed[2] = true;
			break;
		default:
			break;
	}
}

void SceneViewer::mouseReleaseEvent(QMouseEvent* event) {
	switch (event->button()) {
		case Qt::LeftButton:
			m_mousePressed[0] = false;
			break;
		case Qt::MiddleButton:
			m_mousePressed[1] = false;
			break;
		case Qt::RightButton:
			m_mousePressed[2] = false;
			break;
		default:
			break;
	}
}


void SceneViewer::mouseMoveEvent(QMouseEvent* event) {
	
	if(m_altBeingPressed) {
	
		// Viewing Mode

		if(m_mousePressed[0]) {
			qDebug() << event->pos().x() << '|' << m_lastMousePos.x << '|' << m_mousePosWhenPressed.x;
			
			m_manager->getCamera()->rotateAround(m_lastMousePos, { event->pos().x(), event->pos().y() });
			m_lastMousePos.x = event->pos().x();
			m_lastMousePos.y = event->pos().y();
			

		} else if(m_mousePressed[1]) {
		
		} else if(m_mousePressed[2]) {
		
		}

	} else { 

		// Walking Mode
		/*glm::vec2 delta{};
		float intensity = .05f;

		auto center = geometry().center();
		auto globalCenter = mapToGlobal(center);

		delta.x = (event->pos().x() - center.x()) * intensity;
		delta.y = (center.y() - event->pos().y()) * intensity;

		m_manager->getCamera()->addRotation(delta.x, delta.y);
		cursor().setPos(globalCenter.x(), globalCenter.y());*/
	}
}