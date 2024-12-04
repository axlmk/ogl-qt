#include "TriangleWindow.hpp"

void TriangleWindow::initialize() {
	m_program = new QOpenGLShaderProgram();

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "pass_through.vert")) {
		qDebug() << "Vertex shader errors:\n" << m_program->log();
	}

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "uniform_color.frag")) {
		qDebug() << "Fragment shader errors:\n" << m_program->log();
	}

	if (!m_program->link()) {
		qDebug() << "Shader linker errors:\n" << m_program->log();
	}

	float triangle[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	m_vertexBufferObject = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vertexBufferObject.create();
	m_vertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vertexBufferObject.bind();
	m_vertexBufferObject.allocate(triangle, sizeof(triangle));

	m_vao.create();
	m_vao.bind();

	m_program->enableAttributeArray(0);
	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);

	m_vertexBufferObject.release();
	m_vao.release();
}

void TriangleWindow::render() {
	glViewport(0, 0, width(), height());

	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_program->bind();
	m_vao.bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);

	m_vao.release();
}

TriangleWindow::~TriangleWindow() {

	m_context->makeCurrent(this);

	m_vao.destroy();
	m_vertexBufferObject.destroy();
	delete m_program;
}