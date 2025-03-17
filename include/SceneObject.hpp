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

enum LightType
{
	Spot,
	Point,
	Directional
};

struct LightProperties
{
	LightType type;

	// Point light
	float linear;
	float quadratic;

	// Spot light;
	float cutoff;
	float outerCutoff;

	// Directional light & Spot
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
	
	void setDirectionalLight(glm::vec3 direction);
	void setPointLight(float linear, float quadratic);
	void setSpotLight(glm::vec3 direction, float cutoff, float outerCutoff = 0.0);
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
