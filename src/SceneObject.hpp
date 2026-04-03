#pragma once
#include <filesystem>
#include <functional>

#include "Camera.hpp"
#include "Model.hpp"
#include "Shader.hpp"

struct Selection
{
	Shader* color;
	float scale;
};

class Light;

class SceneObject
{
   public:
	std::string debugName;

	SceneObject(Selection selection);
	SceneObject(std::unique_ptr<Model> geometry, std::unique_ptr<Shader> shader, Selection selection = {nullptr, 0.0});

	void setShader(std::unique_ptr<Shader> shader);
	void setModel(std::unique_ptr<Model> model);
	Model* getModel() const;

	glm::vec3 getPosition() const;

	void render(const Camera& camera, const std::vector<Light>& lights) const;

	void renderPicking(const Camera& camera);
	void select();
	void unselect();
	bool isId(glm::ivec3 id) const;

   private:
	void _setUpLights(const Camera& camera, const std::vector<Light>& lights) const;
	glm::vec3 nameToColor();

	std::unique_ptr<Model> m_model;
	std::unique_ptr<Shader> m_shd;
	glm::ivec3 m_colorId;

	Shader* m_pick;

	bool m_isSelected;
	Selection m_selectionData;
};
