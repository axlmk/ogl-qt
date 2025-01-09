#include "Renderable.hpp"

Renderable::Renderable() : m_geo{ nullptr }, m_shd{ nullptr } {
	m_vbo = 0;
	m_vao = 0;
	m_ebo = 0;
	m_err_msg = "";
}

Renderable::Renderable(std::shared_ptr<Geometry> geometry, std::shared_ptr<Shader> shader) {
	linkGeo(geometry);
	linkShader(shader);
}

Renderable::~Renderable() {
	g_opengl.glBindBuffer(GL_ARRAY_BUFFER, 0);
	g_opengl.glDeleteBuffers(1, &m_vbo);
	m_vbo = 0;

	g_opengl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	g_opengl.glDeleteBuffers(1, &m_ebo);
	m_ebo = 0;

	g_opengl.glBindVertexArray(0);
	g_opengl.glDeleteVertexArrays(1, &m_vao);
	m_vao = 0;
}

void Renderable::linkGeo(std::shared_ptr<Geometry> geometry) {
	
	m_geo = geometry;

	if(m_geo->getVertices().empty()) {
		qCritical() << "An empty geometry can't be added to a renderable object";
		return;
	}

	/* Geometry transformation into float's array for OpenGL to render */

	std::vector<Vertex> vertices = m_geo->getVertices();
	std::vector<float> floatVertices;
	floatVertices.reserve(vertices.size() * VERTEX_FULL_SIZE);

	for (size_t i = 0; i < vertices.size(); i++) {
		std::vector<float> tmpFloat = static_cast<std::vector<float>>(vertices[i]);
		std::copy(tmpFloat.begin(), tmpFloat.end(), std::back_inserter(floatVertices));
	}

	/* Vertex Array Object (VAO) management */

	g_opengl.glGenVertexArrays(1, &m_vao);
	g_opengl.glBindVertexArray(m_vao);

	/* Vertex Buffer Object (VBO) management */

	g_opengl.glGenBuffers(1, &m_vbo);
	g_opengl.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	g_opengl.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatVertices.size(), floatVertices.data(), GL_STATIC_DRAW);

	/* Element Buffer Object (EBO) management */

	g_opengl.glGenBuffers(1, &m_ebo);
	g_opengl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	g_opengl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, VERTICE_LINK_SIZEOF * m_geo->getVerticesLink().size(), m_geo->getVerticesLink().data(), GL_STATIC_DRAW);

	/* Specify vertex attribute format */

	std::vector<unsigned int> attributesPositions = m_geo->getAttributesPositions();
	unsigned int currentAttributePosition = 0;
	for (size_t i = 0; i < attributesPositions.size(); i++) {
		g_opengl.glVertexAttribPointer(i, attributesPositions[i], GL_FLOAT, GL_FALSE, attributesPositions[i] * sizeof(float), (void*) currentAttributePosition);
		currentAttributePosition += attributesPositions[i];
		g_opengl.glEnableVertexAttribArray(i);
	}
	
	return;
}

void Renderable::linkShader(std::shared_ptr<Shader> shader) {
	m_shd = shader;
}

void Renderable::render() const {
	if(m_shd != nullptr)
		m_shd->use();
	else
		qWarning() << "Object is rendered without a shader attached";
	g_opengl.glBindVertexArray(m_vao);
	g_opengl.glDrawElements(GL_TRIANGLES, m_geo->getVerticesLink().size(), GL_UNSIGNED_INT, 0);
	g_opengl.glBindVertexArray(0);
}
