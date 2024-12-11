#include "RectangleWindow.hpp"

RectangleWindow::~RectangleWindow() {

	m_vao.destroy();
	m_vertexBufferObject.destroy();
	delete m_program;
}

void RectangleWindow::initializeGL() {
	initializeOpenGLFunctions();

	float renderedObject[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
}

void RectangleWindow::paintGL() {

	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
