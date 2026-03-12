#pragma once
#include "SceneObject.hpp"
#include "glm/glm.hpp"

class Gizmo
{
   public:
	Gizmo(std::unique_ptr<SceneObject> red, std::unique_ptr<SceneObject> green, std::unique_ptr<SceneObject> blue);
	void render(const Camera& camera, const SceneObject* selectedObject,
				const std::vector<std::reference_wrapper<SceneObject>>& lights) const;

	void renderPicking(const Camera& camera, const SceneObject* selectedObject);

	bool isId(glm::ivec3 id);

	void setPosition(const glm::vec3& position);

	bool isSelected(void) const;

	int getSelectedIndex(void) const;

	SceneObject* getSelectedArrow(void) const;

   private:
	int m_selectedArrow;
	std::array<std::unique_ptr<SceneObject>, 3> m_arrows;
	glm::vec3 m_localisation;
};