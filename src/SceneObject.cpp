#include "SceneObject.hpp"

#include "Light.hpp"

SceneObject::SceneObject(Selection selection)
	: debugName{}, m_model{nullptr}, m_shd{nullptr}, m_pick{new Shader(ShaderType::Unicolor)}, m_isSelected{false}, m_selectionData{selection}
{}

SceneObject::SceneObject(std::unique_ptr<Model> model, std::unique_ptr<Shader> shader, Selection selection) : SceneObject(selection)
{
	m_model = std::move(model);
	m_shd = std::move(shader);
}

Model* SceneObject::getModel() const
{
	return m_model.get();
}

void SceneObject::setModel(std::unique_ptr<Model> model)
{
	m_model = std::move(model);
}

void SceneObject::setShader(std::unique_ptr<Shader> shader)
{
	m_shd = std::move(shader);
}

void SceneObject::_setUpLights(const Camera& camera, const std::vector<Light>& lights) const
{
	int uniform = m_shd->getUniform("nLights");
	g_opengl.glUniform1i(uniform, (int)lights.size());

	int i = 0;
	for (const auto& light : lights)
	{
		std::string iStr = std::to_string(i++);
		const auto properties = light.getProperties();

		uniform = m_shd->getUniform("lights[" + iStr + "].type");
		g_opengl.glUniform1i(uniform, properties.type);

		switch (properties.type)
		{
			case LightType::Point: {

				uniform = m_shd->getUniform("lights[" + iStr + "].position");
				g_opengl.glUniform3f(uniform, light.getPosition().x, light.getPosition().y, light.getPosition().z);

				uniform = m_shd->getUniform("lights[" + iStr + "].linear");
				g_opengl.glUniform1f(uniform, properties.linear);

				uniform = m_shd->getUniform("lights[" + iStr + "].quadratic");
				g_opengl.glUniform1f(uniform, properties.quadratic);
				break;
			}
			case LightType::Spot: {
				uniform = m_shd->getUniform("lights[" + iStr + "].cutoff");
				g_opengl.glUniform1f(uniform, glm::cos(glm::radians(properties.cutoff)));

				uniform = m_shd->getUniform("lights[" + iStr + "].outerCutoff");
				g_opengl.glUniform1f(uniform, glm::cos(glm::radians(properties.outerCutoff)));

				uniform = m_shd->getUniform("lights[" + iStr + "].position");
				g_opengl.glUniform3f(uniform, light.getPosition().x, light.getPosition().y, light.getPosition().z);
				[[fallthrough]];
			}
			case LightType::Directional: {
				uniform = m_shd->getUniform("lights[" + iStr + "].direction");
				g_opengl.glUniform3f(uniform, properties.direction.x, properties.direction.y, properties.direction.z);
				break;
			}
			default: {
				break;
			}
		}

		uniform = m_shd->getUniform("lights[" + iStr + "].ambient");
		g_opengl.glUniform3f(uniform, 0.2f, 0.2f, 0.2f);

		uniform = m_shd->getUniform("lights[" + iStr + "].diffuse");
		g_opengl.glUniform3f(uniform, 0.65f, 0.65f, 0.65f);

		uniform = m_shd->getUniform("lights[" + iStr + "].specular");
		g_opengl.glUniform3f(uniform, 1.0f, 1.0f, 1.0f);
	}

	SpaceCoord cameraPos = camera.getPosition();
	uniform = m_shd->getUniform("cameraPos");
	g_opengl.glUniform3f(uniform, cameraPos.x, cameraPos.y, cameraPos.z);
}

glm::vec3 SceneObject::getPosition() const
{
	return m_model->getPosition();
}

void SceneObject::render(const Camera& camera, const std::vector<Light>& lights) const
{
	if (m_shd == nullptr || m_model == nullptr)
	{
		return;
	}

	glm::mat4 model = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 projection = glm::mat4(1.f);

	if (m_isSelected)
	{
		g_opengl.glStencilMask(0xFF);
		g_opengl.glStencilFunc(GL_ALWAYS, 1, 0xFF);
	}
	else
	{
		g_opengl.glStencilMask(0x00);
	}

	m_shd->use();

	// World's location
	model = m_model->getTransforms();

	// Camera's location
	view = camera.getSpaceMat();

	// Camera's settings)
	projection = glm::perspective(glm::radians(camera.getFov()), 600.f / 400.f, camera.getNearPlan(), camera.getFarPlan());

	// Combination of previous views
	m_shd->setTransformation(model, view, projection);

	// Lights calculations
	if (m_shd->getType() == ShaderType::Texture)
		_setUpLights(camera, lights);

	// Final render
	m_model->Draw(*m_shd);

	// Selection handling
	if (m_isSelected)
	{
		g_opengl.glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		g_opengl.glStencilMask(0x00);
		g_opengl.glDisable(GL_DEPTH_TEST);

		m_selectionData.color->use();

		m_model->scale(m_selectionData.scale);
		model = m_model->getTransforms();
		m_selectionData.color->setTransformation(model, view, projection);
		m_model->Draw(*m_selectionData.color);
		m_model->scale(1.0f);

		g_opengl.glStencilMask(0xFF);
		g_opengl.glStencilFunc(GL_ALWAYS, 1, 0xFF);
		g_opengl.glEnable(GL_DEPTH_TEST);
		g_opengl.glClear(GL_STENCIL_BUFFER_BIT);
	}
}

void SceneObject::renderPicking(const Camera& camera)
{
	if (!m_model)
	{
		return;
	}

	glm::mat4 model = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 projection = glm::mat4(1.f);

	auto idColor = nameToColor();
	m_pick->setColor(idColor);
	m_pick->use();
	// World's location
	model = m_model->getTransforms();
	// Camera's location
	view = camera.getSpaceMat();
	// Camera's settings)
	projection = glm::perspective(glm::radians(camera.getFov()), 600.f / 400.f, camera.getNearPlan(), camera.getFarPlan());
	// Combination of previous views
	m_pick->setTransformation(model, view, projection);

	// Final render
	m_model->Draw(*m_pick);
}

void SceneObject::select()
{
	m_isSelected = true;
}

void SceneObject::unselect()
{
	m_isSelected = false;
}

glm::vec3 SceneObject::nameToColor()
{
	if (debugName == "r")
	{
		m_colorId = {255.0, 0.0, 0.0};
		return {1.0, 0.0, 0.0};
	}
	else if (debugName == "v")
	{
		m_colorId = {0.0, 255.0, 0.0};
		return {0.0, 1.0, 0.0};
	}
	else if (debugName == "b")
	{
		m_colorId = {0.0, 0.0, 255.0};
		return {0.0, 0.0, 1.0};
	}
	else
	{
		std::hash<std::string> hasher;
		int32_t id = static_cast<int32_t>(hasher(debugName));

		m_colorId.x = id & 0xFF;
		m_colorId.y = (id >> 8) & 0xFF;
		m_colorId.z = (id >> 16) & 0xFF;

		return {m_colorId.x / 255.0, m_colorId.y / 255.0, m_colorId.z / 255.0};
	}
}

bool SceneObject::isId(glm::ivec3 id) const
{
	return id == m_colorId;
}