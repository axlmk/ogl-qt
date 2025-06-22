#pragma once
#include "SceneObject.hpp"

class Gizmo {
	public:
		Gizmo(std::unique_ptr<SceneObject> red, std::unique_ptr<SceneObject> green, std::unique_ptr<SceneObject> blue);
		void render(const Camera& camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const;
		void setPosition(const glm::vec3 &position);
	private:
		std::array<std::unique_ptr<SceneObject>, 3> m_arrows;
		glm::vec3 m_localisation;
};