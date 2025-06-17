#include "SceneObject.hpp"

SceneObject::SceneObject(const Selection& selection, SceneObjectType type) : m_model{ nullptr }, m_shd{ nullptr }, m_selectionData{ selection }, m_pick{ new Shader(ShaderType::Unicolor) }
{
	m_type = type;

	if(m_type == SceneObjectType::Light)
	{
		m_lightProperties.type = LightType::Directional;
		m_model = new Model("resources/models/sphere/sphere.obj");
		m_shd = new Shader(ShaderType::Light);
		m_shd->setLight();
	}
}



SceneObject::SceneObject(Model* model, Shader* shader, const Selection &selection, SceneObjectType type) : SceneObject(selection, type)
{
	linkModel(model);
	linkShader(shader);
}


Model* SceneObject::getModel() const
{
	return m_model;
}


Shader* SceneObject::getShader() const
{
	return m_shd;
}



void SceneObject::linkModel(Model *model)
{
	m_model = model;
}

void SceneObject::linkShader(Shader* shader)
{
	m_shd = shader;
}



void SceneObject::setSpotLight(glm::vec3 direction, float cutoff, float outerCutoff)
{
	m_lightProperties.type = LightType::Spot;
	m_lightProperties.cutoff = cutoff;
	m_lightProperties.direction = direction;
	m_lightProperties.outerCutoff = outerCutoff;
}



void SceneObject::setPointLight(float linear, float quadratic)
{
	m_lightProperties.type = LightType::Point;
	m_lightProperties.linear = linear;
	m_lightProperties.quadratic = quadratic;
}



void SceneObject::setDirectionalLight(glm::vec3 direction)
{
	m_lightProperties.type = LightType::Point;
	m_lightProperties.direction = direction;
}



void SceneObject::setUpLights(const Camera &camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const
{
	size_t n = lights.size();
	int uniform = m_shd->getUniform("nLights");;
	g_opengl.glUniform1i(uniform, (int)n);

	for(size_t i = 0; i < n; i++)
	{
		auto& light = lights[i].get();
		std::string iStr = std::to_string(i);

		uniform = m_shd->getUniform("lights[" + iStr + "].type");
		g_opengl.glUniform1i(uniform, light.m_lightProperties.type);

		switch (light.m_lightProperties.type)
		{
			case LightType::Point:
			{

				uniform = m_shd->getUniform("lights[" + iStr + "].position");
				g_opengl.glUniform3f(uniform, light.getPosition().x, light.getPosition().y, light.getPosition().z);

				uniform = m_shd->getUniform("lights[" + iStr + "].linear");
				g_opengl.glUniform1f(uniform, light.m_lightProperties.linear);

				uniform = m_shd->getUniform("lights[" + iStr + "].quadratic");
				g_opengl.glUniform1f(uniform, light.m_lightProperties.quadratic);
				break;
			}
			case LightType::Spot:
			{
				uniform = m_shd->getUniform("lights[" + iStr + "].cutoff");
				g_opengl.glUniform1f(uniform, glm::cos(glm::radians(light.m_lightProperties.cutoff)));

				uniform = m_shd->getUniform("lights[" + iStr + "].outerCutoff");
				g_opengl.glUniform1f(uniform, glm::cos(glm::radians(light.m_lightProperties.outerCutoff)));

				uniform = m_shd->getUniform("lights[" + iStr + "].position");
				g_opengl.glUniform3f(uniform, light.getPosition().x, light.getPosition().y, light.getPosition().z);

				// On purpose fallthrough
			}
			case LightType::Directional:
			{
				uniform = m_shd->getUniform("lights[" + iStr + "].direction");
				g_opengl.glUniform3f(uniform, light.m_lightProperties.direction.x, light.m_lightProperties.direction.y, light.m_lightProperties.direction.z);
				break;
			}
			default:
			{
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

void SceneObject::render(const Camera &camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const
{
	if(m_shd == nullptr || m_model == nullptr)
	{
		return;
	}

	glm::mat4 model			= glm::mat4(1.f);
	glm::mat4 view			= glm::mat4(1.f);
	glm::mat4 projection	= glm::mat4(1.f);

	if(m_isSelected)
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
	if(m_shd->getType() == ShaderType::Texture)
		setUpLights(camera, lights);
	
	// Final render
	m_model->Draw(*m_shd);

	// Selection handling
	if(m_isSelected)
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

glm::vec3 SceneObject::nameToColor() {
	std::hash<std::string> hasher;
	int32_t id = static_cast<int32_t>(hasher(debugName));

	m_colorId.x = id & 0xFF;
	m_colorId.y = (id >> 8) & 0xFF;
	m_colorId.z = (id >> 16) & 0xFF;

	return { m_colorId.x / 255.0, m_colorId.y / 255.0, m_colorId.z / 255.0};
}

bool SceneObject::isId(glm::ivec3 id) const {
	return id == m_colorId;
}