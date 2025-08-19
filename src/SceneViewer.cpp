#include "SceneViewer.hpp"
#include "Scene.hpp"



void SceneViewer::paintGL()
{
	m_deltaTime = QDateTime::currentMSecsSinceEpoch() - m_currentTime;
	m_currentTime = QDateTime::currentMSecsSinceEpoch();

	m_manager->renderLoop(m_inputsBeingPressed, m_deltaTime);
}



SceneViewer::SceneViewer(scene* scene) :
	m_inputsBeingPressed	{ },
	m_deltaTime			{ 0 },
	m_lastMousePos		{ 0.0, 0.0 },
	m_lastFrameMousePos{ 0.0, 0.0 },
	m_manager			{ scene },
	m_timer				{ std::make_unique<QTimer>(this) },
	m_currentTime		{ QDateTime::currentMSecsSinceEpoch() }
{
	connect(&(*m_timer), &QTimer::timeout, this, QOverload<>::of(&SceneViewer::update));
	m_timer->start(16);
}



SceneViewer::~SceneViewer() {

}



void SceneViewer::initializeGL()
{
	this->requestActivate();
	g_opengl.initializeOpenGLFunctions();
	
	g_opengl.glEnable(GL_DEPTH_TEST);
	g_opengl.glEnable(GL_STENCIL_TEST);
	g_opengl.glEnable(GL_BLEND);
	g_opengl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_manager->initializeScene();
}



void SceneViewer::keyPressEvent(QKeyEvent* event)
{

	if(event->text() == "z")
		m_inputsBeingPressed["z"] = true;

	if(event->text() == "q")
		m_inputsBeingPressed["q"] = true;

	if(event->text() == "d")
		m_inputsBeingPressed["d"] = true;

	if(event->text() == "s")
		m_inputsBeingPressed["s"] = true;

	if(event->key() == Qt::Key_Alt )
	{
		m_inputsBeingPressed["alt"] = true;
		
		// Reset cursor
		setCursor(QCursor(Qt::BlankCursor));
		m_lastMousePos.x = cursor().pos().x();
		m_lastMousePos.y = cursor().pos().y();
		auto globalCenter = mapToGlobal(geometry().center());
		cursor().setPos(globalCenter.x(), globalCenter.y());
	}
}



void SceneViewer::keyReleaseEvent(QKeyEvent * event) {
	if (event->text() == "z")
		m_inputsBeingPressed["z"] = false;

	if (event->text() == "q")
		m_inputsBeingPressed["q"] = false;

	if (event->text() == "d")
		m_inputsBeingPressed["d"] = false;

	if (event->text() == "s")
		m_inputsBeingPressed["s"] = false;

	if (event->text() == "f")
		m_manager->focusCameraOnSelectedObject();

	if(event->key() == Qt::Key_Alt)
	{
		if (m_inputsBeingPressed["alt"]) {
			m_inputsBeingPressed["alt"] = false;
			setCursor(QCursor(Qt::BitmapCursor));
			cursor().setPos({m_lastMousePos.x, m_lastMousePos.y});
		}
	}
}

void SceneViewer::mousePressEvent(QMouseEvent* event) {
	switch (event->button()) {
		case Qt::LeftButton:
			m_inputsBeingPressed["left"] = true;
			if (!m_inputsBeingPressed["alt"]) {
				m_manager->tryToSelect({ event->pos().x(), event->pos().y() }, geometry().width(), geometry().height());
				m_lastFrameMousePos = glm::ivec2(event->pos().x(), event->pos().y());
			}
			break;
		case Qt::MiddleButton:
			m_inputsBeingPressed["middle"] = true;
			break;
		case Qt::RightButton:
			m_inputsBeingPressed["right"] = true;
			break;
		default:
			break;
	}
}

void SceneViewer::mouseReleaseEvent(QMouseEvent* event) {
	switch (event->button()) {
		case Qt::LeftButton:
			m_inputsBeingPressed["left"] = false;
			m_manager->m_isPicking = false;
			break;
		case Qt::MiddleButton:
			m_inputsBeingPressed["middle"] = false;
			break;
		case Qt::RightButton:
			m_inputsBeingPressed["right"] = false;
			break;
		default:
			break;
	}
}


void SceneViewer::mouseMoveEvent(QMouseEvent* event) {
	
	if(m_inputsBeingPressed["alt"])
	{
		// Viewing Mode
		auto centered = mapToGlobal(geometry().center());
		auto current = mapToGlobal(event->pos());
		glm::vec2 centeredCursor	{ centered.x(), centered.y() };
		glm::vec2 currentCursor		{ current.x(), current.y() };

		if(m_inputsBeingPressed["left"])
			m_manager->getCamera()->rotate(centeredCursor, currentCursor);
		else if(m_inputsBeingPressed["middle"])
			m_manager->getCamera()->pan(centeredCursor, currentCursor);
		else if(m_inputsBeingPressed["right"])
			m_manager->getCamera()->zoom(centeredCursor, currentCursor);

		cursor().setPos(centeredCursor.x, centeredCursor.y);
	}
	else if(m_inputsBeingPressed["left"]) {
		auto currentMousePos = glm::ivec2(event->pos().x(), event->pos().y());
		glm::ivec2 mouseDiff = m_lastFrameMousePos - currentMousePos;
		m_manager->tryMoveObject(mouseDiff);
		m_lastFrameMousePos = currentMousePos;
	}
}