#include "SceneObject.hpp"

SceneObject::SceneObject() : m_geo{ nullptr }, m_shd{ nullptr } {
	m_vbo = 0;
	m_vao = 0;
	m_ebo = 0;
	std::string err_msg = "";
}

SceneObject::SceneObject(Geometry* geometry, Shader* shader) : SceneObject() {
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



Geometry* SceneObject::getGeometry() const {
	return m_geo;
}



Shader* SceneObject::getShader() const {
	return m_shd;
}



void SceneObject::linkGeo(Geometry* geometry) {

	if(m_geo == geometry) {
		qWarning() << "The newly added geometry corresponds to the one actually in use";
		return;
	}

	if(geometry->empty()) {
		qCritical() << "An empty geometry can't be added to a SceneObject object";
		return;
	}

	m_geo = geometry;

	generateRender();
}

void SceneObject::linkShader(Shader* shader) {
	
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
		case ShaderType::Light:
			m_geo->unsetTextureMapping();
			break;
		default:
			std::string err_msg = "ShaderType unknowed";
			qCritical() << err_msg;
			throw std::invalid_argument(err_msg);
	}

	generateRender();
}


void SceneObject::render(Camera* camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const
{
	if(m_shd == nullptr || m_geo == nullptr)
		return;
	
	// Shader's application
	m_shd->use();

	glm::mat4 model			= glm::mat4(1.f);
	glm::mat4 view			= glm::mat4(1.f);
	glm::mat4 projection	= glm::mat4(1.f);

	// Model (World's location)
	/*glm::vec4 modelRotation = m_geo->getRotation();
	model = glm::rotate(model, modelRotation.x, {modelRotation.y, modelRotation.z, modelRotation.w});*/
	model = glm::translate(model, m_geo->getPosition());
	
	// View (camera's location)
	view = camera->getSpaceMat();

	// Projection (camera's settings)
	projection = glm::perspective(glm::radians(camera->getFov()), 600.f / 400.f, camera->getNearPlan(), camera->getFarPlan());
	
	// Clip (combination of previous matrices)
	m_shd->setTransformation(model, view, projection);

	if(this != &lights[0].get())
	{
		unsigned int lightPosUni = m_shd->getUniform("lightPos");
		SpaceCoord lightPos = lights[0].get().getGeometry()->getPosition();
		g_opengl.glUniform3f(lightPosUni, lightPos.x, lightPos.y, lightPos.z);
	}

	// Render
	g_opengl.glBindVertexArray(m_vao);
	g_opengl.glDrawElements(GL_TRIANGLES, m_geo->getVerticesLink().size(), GL_UNSIGNED_INT, 0);
}


void SceneObject::generateRender() {

	// Pretests

	if(m_geo == nullptr || m_shd == nullptr) {
		std::string err_msg = "Geometry and shader must be specify to render a scene object";
		qWarning() << err_msg;
		return;
	}

	// Buffer generation

	g_opengl.glGenVertexArrays(1, &m_vao);
	g_opengl.glGenBuffers(1, &m_vbo);
	g_opengl.glGenBuffers(1, &m_ebo);
	
	// Bind VAO before anything else
	
	g_opengl.glBindVertexArray(m_vao);

	// Send data to graphic card


	std::vector<float> floatVertices = m_geo->getFloatVertices();
	g_opengl.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	g_opengl.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatVertices.size(), floatVertices.data(), GL_STATIC_DRAW);

	// EBO management

	g_opengl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	g_opengl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, VERTICE_LINK_SIZEOF * m_geo->getVerticesLink().size(), m_geo->getVerticesLink().data(), GL_STATIC_DRAW);

	// Handle data between shaders and CPU

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