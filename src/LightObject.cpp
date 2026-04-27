#include "LightObject.hpp"

#include "Camera.hpp"
#include "SceneObject.hpp"

LightObject::LightObject(Model* geometry, Shader* shader) : SceneObject(geometry, shader) {}

void LightObject::render(const Camera& camera, const std::vector<LightProperties*>& lights) const
{
	SceneObject::render(camera, lights);
}

LightProperties* LightObject::getLightProperties(void)
{
	return &m_lightProperties;
}

void LightObject::translate(const glm::vec3& translation)
{
	m_lightProperties.position += translation;
	SceneObject::translate(translation);
}

void LightObject::setDirectionalLight(glm::vec3 direction)
{
	m_lightProperties.type = LightProperties::LightType::Directional;
	m_lightProperties.direction = direction;
}

void LightObject::setPointLight(float linear, float quadratic)
{
	m_lightProperties.type = LightProperties::LightType::Point;
	m_lightProperties.linear = linear;
	m_lightProperties.quadratic = quadratic;
}

void LightObject::setSpotLight(glm::vec3 direction, float cutoff, float outerCutoff)
{
	m_lightProperties.type = LightProperties::LightType::Spot;
	m_lightProperties.cutoff = cutoff;
	m_lightProperties.direction = direction;
	m_lightProperties.outerCutoff = outerCutoff;
}