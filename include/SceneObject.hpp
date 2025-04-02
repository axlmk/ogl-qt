#pragma once
#include <filesystem>
#include "Shader.hpp"
#include "Model.hpp"
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
	std::string debugName;
	SceneObject(SceneObjectType type = SceneObjectType::Normal);
	SceneObject(Model* geometry, Shader* shader, SceneObjectType type = SceneObjectType::Normal);

	Model*	getModel() const;
	Shader*	getShader() const;

	void linkShader(Shader *shader);
	void linkModel(Model *model);
	
	void setDirectionalLight(glm::vec3 direction);
	void setPointLight(float linear, float quadratic);
	void setSpotLight(glm::vec3 direction, float cutoff, float outerCutoff = 0.0);
	void setUpLights(const Camera &camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const;
	
	void render(const Camera &camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const;

private:

	SceneObjectType m_type;
	LightProperties m_lightProperties;

	glm::vec3 m_position;
	glm::vec3 m_direction;

	Model* m_model;
	Shader* m_shd;
};
