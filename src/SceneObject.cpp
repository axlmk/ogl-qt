#include "SceneObject.hpp"

SceneObject::SceneObject(SceneObjectType type) : m_model{ nullptr }, m_shd{ nullptr }
{
	m_type = type;

	if(m_type == SceneObjectType::Light)
	{
		m_lightProperties.type = LightType::Directional;
	}
}



SceneObject::SceneObject(Model* model, Shader* shader, SceneObjectType type) : SceneObject(type)
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
	m_lightProperties.type = LightType::Directional;
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
				g_opengl.glUniform3f(uniform, light.m_position.x, light.m_position.y, light.m_position.z);

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
				g_opengl.glUniform3f(uniform, light.m_position.x, light.m_position.y, light.m_position.z);

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


void SceneObject::render(const Camera &camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const
{
	if(m_shd == nullptr || m_model == nullptr)
	{
		return;
	}
	
	// Shader's application
	m_shd->use();

	glm::mat4 model			= glm::mat4(1.f);
	glm::mat4 view			= glm::mat4(1.f);
	glm::mat4 projection	= glm::mat4(1.f);

	// World's location
	model = m_model->getTransforms();
	
	// Camera's location
	view = camera.getSpaceMat();

	// Camera's settings)
	projection = glm::perspective(glm::radians(camera.getFov()), 600.f / 400.f, camera.getNearPlan(), camera.getFarPlan());
	
	// Combination of previous views
	m_shd->setTransformation(model, view, projection);

	// Lights calculations
	if(m_type != SceneObjectType::Light)
	{
		if(m_shd->getType() == ShaderType::Texture)
			setUpLights(camera, lights);
		// Final render
		m_model->Draw(*m_shd); // temporary
	}

}