#include "SceneObject.hpp"

SceneObject::SceneObject() : m_geo{ nullptr }, m_shd{ nullptr } {
	m_vbo = 0;
	m_vao = 0;
	m_ebo = 0;
	m_err_msg = "";
}

SceneObject::SceneObject(std::shared_ptr<Geometry> geometry, std::shared_ptr<Shader> shader) : SceneObject() {
	linkGeo(geometry);
	linkShader(shader);
}



SceneObject::~SceneObject() {
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



std::shared_ptr<Geometry> SceneObject::getGeometry() const {
	return m_geo;
}



std::shared_ptr<Shader> SceneObject::getShader() const {
	return m_shd;
}



void SceneObject::linkGeo(std::shared_ptr<Geometry> geometry) {

	if(m_geo == geometry) {
		qWarning() << "The newly added geometry corresponds to the one actually in use";
		return;
	}

	if(geometry->empty()) {
		qCritical() << "An empty geometry can't be added to a SceneObject object";
		return;
	}

	m_geo = geometry;
}

void SceneObject::linkShader(std::shared_ptr<Shader> shader) {
	
	if(m_shd == shader) {
		qWarning() << "The newly added shader corresponds to the one actually in use";
		return;
	}
	
	m_shd = shader;

	switch (m_shd->getShaderType()) {
		case ShaderType::Texture:
			m_geo->setTextureMapping();
			break;
		case ShaderType::Unicolor:
			m_geo->unsetTextureMapping();
			break;
		default:
			m_err_msg = "ShaderType unknowed";
			qCritical() << m_err_msg;
			throw std::invalid_argument(m_err_msg);
	}
}


void SceneObject::render(std::shared_ptr<Camera> camera) const {

	if(m_shd == nullptr)
		return;

	if(m_geo == nullptr)
		return;

	m_shd->use();

	/* Coordinates transformation */

	glm::mat4 model = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 projection = glm::mat4(1.f);
	glm::mat4 clip = glm::mat4(1.f);

	model = glm::translate(model, m_geo->getPosition());
	glm::vec4 modelRotation = m_geo->getRotation();
	model = glm::rotate(model, modelRotation.x, {modelRotation.y, modelRotation.z, modelRotation.w});
	
	view = camera->getSpaceMat();

	projection = glm::perspective(glm::radians(camera->getFov()), 600.f / 400.f, camera->getNearPlan(), camera->getFarPlan());
	clip = projection * view * model;
	
	m_shd->setTransformation(clip);
	
	g_opengl.glBindVertexArray(m_vao);
	g_opengl.glDrawElements(GL_TRIANGLES, m_geo->getVerticesLink().size(), GL_UNSIGNED_INT, 0);
}


void SceneObject::generateRender() {

	/* Pretests */

	if(m_geo == nullptr || m_shd == nullptr) {
		m_err_msg = "Vertices could not be interpreted if the geometry hasn't been set";
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}

	// We configure the geometry according to the type of the associated shader

	if (m_shd->getShaderType() == ShaderType::Texture) {
		m_geo->setTextureMapping();
	}

	// Element Buffer Object (EBO) management

	g_opengl.glGenBuffers(1, &m_ebo);
	g_opengl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	g_opengl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, VERTICE_LINK_SIZEOF * m_geo->getVerticesLink().size(), m_geo->getVerticesLink().data(), GL_STATIC_DRAW);

	// Vertex Array Object (VAO) management

	g_opengl.glGenVertexArrays(1, &m_vao);

	// Vertex Buffer Object (VBO) management

	g_opengl.glGenBuffers(1, &m_vbo);

	/* Geometry transformation into float's array for OpenGL to render */

	std::vector<float> floatVertices = m_geo->getFloatVertices();

	/* Transmission of the data to OpenGL */
	g_opengl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	g_opengl.glBindVertexArray(m_vao);
	g_opengl.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	g_opengl.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatVertices.size(), floatVertices.data(), GL_STATIC_DRAW);

	/* Set data for draw call */

	std::vector<unsigned int> attributesPositions = m_geo->getAttributesPositions();
	unsigned int currentAttributePosition = 0;
	unsigned int stride = std::accumulate(attributesPositions.begin(), attributesPositions.end(), 0);

	for (size_t i = 0; i < attributesPositions.size(); i++) {
		g_opengl.glVertexAttribPointer(i, attributesPositions[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(currentAttributePosition * sizeof(float)));
		currentAttributePosition += attributesPositions[i];
		g_opengl.glEnableVertexAttribArray(i);
	}
}



void SceneObject::deleteBuffers() {

	g_opengl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	g_opengl.glDeleteBuffers(1, &m_ebo);
	m_ebo = 0;

	g_opengl.glBindVertexArray(0);
	g_opengl.glDeleteVertexArrays(1, &m_vao);
	m_vao = 0;

	g_opengl.glBindBuffer(GL_ARRAY_BUFFER, 0);
	g_opengl.glDeleteBuffers(1, &m_vbo);
	m_vbo = 0;
}