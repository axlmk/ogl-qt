#include "SceneViewer.hpp"
#include "Scene.hpp"



void SceneViewer::paintGL()
{
	static int i = 0;
	static auto fpsSmoother = 600;
	static std::string smoothDT = "";

	m_deltaTime = QDateTime::currentMSecsSinceEpoch() - m_currentTime;
	m_currentTime = QDateTime::currentMSecsSinceEpoch();
	fpsSmoother += m_deltaTime;

	g_opengl.glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto &sceneObject : m_manager->getSceneObjects())
	{
		m_manager->getCamera()->move(m_KeyBeingPressed, m_deltaTime);
		m_manager->getGeometries()[1]->rotate(i * 0.01, 0, 1, 0);
		sceneObject->render(m_manager->getCamera().get(), m_manager->getLights());
	}

	for(auto &hud : m_manager->getHUDs())
	{
		if(fpsSmoother > 600)
		{
			smoothDT = std::to_string(int(1000 / m_deltaTime));
			fpsSmoother = 0;
		}
		hud->RenderText(smoothDT, 1, 1, {255, 255, 255}, 0.5);
	}
	i++;
}



SceneViewer::SceneViewer(scene* scene) :
	m_deltaTime			{ 0 },
	m_lastMousePos		{ 0.0, 0.0 },
	m_mousePosWhenPressed{ 0.0, 0.0 },
	m_altBeingPressed	{ false },
	m_mousePressed		{ false, false, false },
	m_KeyBeingPressed	{ false, false, false, false },
	m_manager			{ scene },
	m_timer				{ std::make_unique<QTimer>(this) },
	m_currentTime		{ QDateTime::currentMSecsSinceEpoch() } {

	connect(&(*m_timer), &QTimer::timeout, this, QOverload<>::of(&SceneViewer::update));
	m_timer->start(16);
}



SceneViewer::~SceneViewer() {

}



void SceneViewer::initializeGL()
{
	g_opengl.initializeOpenGLFunctions();
	
	g_opengl.glEnable(GL_DEPTH_TEST);
	g_opengl.glEnable(GL_BLEND);
	g_opengl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_manager->initializeScene();
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
	m_lastMousePos.x = event->pos().x();
	m_lastMousePos.y = event->pos().y();
	m_mousePosWhenPressed = m_lastMousePos;
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
	
	if(m_altBeingPressed)
	{
		// Viewing Mode

		if(m_mousePressed[0])
			m_manager->getCamera()->rotate(m_lastMousePos, { event->pos().x(), event->pos().y() });
		else if(m_mousePressed[1])
			m_manager->getCamera()->pan(m_lastMousePos, { event->pos().x(), event->pos().y() });
		else if(m_mousePressed[2])
			m_manager->getCamera()->zoom(m_lastMousePos, { event->pos().x(), event->pos().y() });
		m_lastMousePos.x = event->pos().x();
		m_lastMousePos.y = event->pos().y();

	}
}