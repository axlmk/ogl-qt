#include "Light.hpp"

// todo implementer les fonctions du header

Light::Light(Selection selection) : SceneObject(selection)
{
	m_lightProperties.type = LightType::Directional;
	m_model = new Model("resources/models/sphere/sphere.obj");
	m_shd = new Shader(ShaderType::Light);
	m_shd->setLight();
}