#pragma once
#include <filesystem>
#include "Shader.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"

enum class SceneObjectType
{
	Normal,
	Light
};

enum class LightType
{
	Directional,
	Point,
	Spot
};

struct LightProperties
{
	LightType type;

	// Point light
	float linear;
	float quadratic;

	// Spot light;
	float cutoff;

	// Directional light
	glm::vec3 direction;
};

class SceneObject {
public:

	SceneObject(SceneObjectType type = SceneObjectType::Normal);
	SceneObject(Geometry* geometry, Shader* shader, SceneObjectType type = SceneObjectType::Normal);
	~SceneObject();

	Geometry*	getGeometry() const;
	Shader*		getShader() const;

	void generateRender();
	void linkShader(Shader* shader);
	void linkGeo(Geometry* geometry);
	void setLightProperties(LightType type, glm::vec3 direction = {0, 0, 0}, float linear = 1, float quadratic = 1, float cutoff = 1);

	void setUpLights(Camera* camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const;
	void render(Camera* camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const;

private:

	unsigned int m_vbo;
	unsigned int m_vao;
	unsigned int m_ebo;

	SceneObjectType m_type;
	LightProperties m_lightProperties;

	Geometry* m_geo;
	Shader* m_shd;

	void deleteBuffers();
};
