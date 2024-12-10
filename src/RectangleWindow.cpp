#include "RectangleWindow.hpp"

RectangleWindow::~RectangleWindow() {

	m_vao.destroy();
	m_vertexBufferObject.destroy();
	delete m_program;
}

void RectangleWindow::initializeGL() {
	m_program = new QOpenGLShaderProgram();

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/pass_through.vert")) {
		qDebug() << "Vertex shader errors:\n" << m_program->log();
	}

	if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/uniform_color.frag")) {
		qDebug() << "Fragment shader errors:\n" << m_program->log();
	}

	if (!m_program->link()) {
		qDebug() << "Shader linker errors:\n" << m_program->log();
	}

	float rectangle[] = {
		 0.8f,  0.8f, 0.0f,  // top right
		 0.8f, -0.8f, 0.0f,  // bottom right
		-0.8f, -0.8f, 0.0f,  // bottom left
		-0.8f,  0.8f, 0.0f
	};

	QColor vertexColors[] = {
		QColor("#f6a509"),
		QColor("#cb2dde"),
		QColor("#0eeed1"),
		QColor("#068918"),
	};

	std::vector<float> vertexBufferData(2 * 4 * 3);
	// create new data buffer - the following memory copy stuff should
	// be placed in some convenience class in later tutorials
	// copy data in interleaved mode with pattern p0c0|p1c1|p2c2|p3c3
	float* buf = vertexBufferData.data();
	for (int v = 0; v < 4; ++v, buf += 6) {
		// coordinates
		buf[0] = rectangle[3 * v];
		buf[1] = rectangle[3 * v + 1];
		buf[2] = rectangle[3 * v + 2];
		// colors
		buf[3] = vertexColors[v].redF();
		buf[4] = vertexColors[v].greenF();
		buf[5] = vertexColors[v].blueF();
	}

	m_vertexBufferObject = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vertexBufferObject.create();
	m_vertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vertexBufferObject.bind();
	m_vertexBufferObject.allocate(vertexBufferData.data(), vertexBufferData.size() * sizeof(float));

	m_vao.create();
	m_vao.bind();

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// create a new buffer for the indexes
	m_indexBufferObject = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer); // Mind: use 'IndexBuffer' here
	m_indexBufferObject.create();
	m_indexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_indexBufferObject.bind();
	m_indexBufferObject.allocate(indices, sizeof(indices));


	// stride = number of bytes for one vertex (with all its attributes) = 3+3 floats = 6*4 = 24 Bytes
	int stride = 6 * sizeof(float);

	// layout location 0 - vec3 with coordinates
	m_program->enableAttributeArray(0);
	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, stride);

	// layout location 1 - vec3 with colors
	m_program->enableAttributeArray(1);
	int colorOffset = 3 * sizeof(float);
	m_program->setAttributeBuffer(1, GL_FLOAT, colorOffset, 3, stride);

	m_vertexBufferObject.release();
}

void RectangleWindow::paintGL() {

	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_program->bind();
	m_vao.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	m_vao.release();
}
