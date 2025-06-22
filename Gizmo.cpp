#include "gizmo.hpp"

Gizmo::Gizmo(std::unique_ptr<SceneObject> red, std::unique_ptr<SceneObject> green, std::unique_ptr<SceneObject> blue) :
	m_arrows{std::move(red), std::move(blue), std::move(green)}
{

}

void Gizmo::render(const Camera& camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const {
	m_arrows[0]->render(camera, lights);
	m_arrows[1]->render(camera, lights);
	m_arrows[2]->render(camera, lights);
}

void Gizmo::setPosition(const glm::vec3& position) {
	m_arrows[0]->getModel()->setPosition(position);
	m_arrows[1]->getModel()->setPosition(position);
	m_arrows[2]->getModel()->setPosition(position);
}