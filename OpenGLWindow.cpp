#include "OpenGLWindow.hpp"

OpenGLWindow::OpenGLWindow(QWindow* parent) : QWindow(parent), m_context{nullptr} {
	setSurfaceType(QWindow::OpenGLSurface);
}

void OpenGLWindow::exposeEvent(QExposeEvent* event) {
	renderNow();
}

bool OpenGLWindow::event(QEvent* event) {
	switch (event->type()) {
		case QEvent::UpdateRequest:
			renderNow();
			return true;
		default:
			return QWindow::event(event);
	}
}
void OpenGLWindow::renderLater() {
	requestUpdate();
}

void OpenGLWindow::renderNow() {
	if(!isExposed())
		return;

	bool needsInitialize = false;

	if(m_context == nullptr) {
		m_context = new QOpenGLContext(this);
		m_context->setFormat(requestedFormat());
		m_context->create();

		needsInitialize = true;
	}	

	m_context->makeCurrent(this);

	if(needsInitialize) {
		initializeOpenGLFunctions();
		initialize();
	}

	render();

	m_context->swapBuffers(this);
}