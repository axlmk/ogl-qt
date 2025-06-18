#pragma once
#include <filesystem>
#include <functional>
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
	LightType	type;
	float		linear;
	float		quadratic;
	glm::vec3	direction;
	float		cutoff;
	float		outerCutoff;
};

struct Selection
{
	Shader	*color;
	float	scale;
};

class SceneObject {
public:

	std::string debugName;
	SceneObject(Selection selection, SceneObjectType type = SceneObjectType::Normal);
	SceneObject(Model* geometry, Shader* shader, Selection selection, SceneObjectType type = SceneObjectType::Normal);

	void linkShader(Shader *shader);
	void linkModel(Model *model);

	Model*	getModel() const;
	Shader*	getShader() const;

	glm::vec3 getPosition() const;
	
	void setDirectionalLight(glm::vec3 direction);
	void setPointLight(float linear, float quadratic);
	void setSpotLight(glm::vec3 direction, float cutoff, float outerCutoff = 0.0);
	void setUpLights(const Camera &camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const;
	
	void render(const Camera &camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const;
	void renderPicking(const Camera& camera);
	void select();
	void unselect();
	bool isId(glm::ivec3 id) const;

private:
	
	glm::vec3 nameToColor();

	SceneObjectType m_type;
	LightProperties m_lightProperties;
	
	Model	*m_model;
	Shader	*m_shd;
	glm::ivec3 m_colorId;

	Shader* m_pick;

	bool		m_isSelected;
	Selection	m_selectionData;
};
