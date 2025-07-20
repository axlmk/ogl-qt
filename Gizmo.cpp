#include "gizmo.hpp"

Gizmo::Gizmo(std::unique_ptr<SceneObject> red, std::unique_ptr<SceneObject> green, std::unique_ptr<SceneObject> blue) :
	m_arrows{std::move(red), std::move(green), std::move(blue)}
	, m_selectedArrow{ -1 }
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

void Gizmo::renderPicking(const Camera& camera) {
	m_arrows[0]->renderPicking(camera);
	m_arrows[1]->renderPicking(camera);
	m_arrows[2]->renderPicking(camera);
}


bool Gizmo::isId(const glm::ivec3 id) {
	if (m_arrows[0]->isId(id)) {
		m_selectedArrow = 0;
	}
	else if (m_arrows[1]->isId(id)) {
		m_selectedArrow = 1;
	}
	else if (m_arrows[2]->isId(id)) {
		m_selectedArrow = 2;
	}
	else {
		m_selectedArrow = -1;
		return false;
	}
	return true;
}

SceneObject* Gizmo::getSelectedArrow(void) const {
	if (m_selectedArrow >= 0 && m_selectedArrow < 3) {
		return m_arrows[m_selectedArrow].get();
	}
	return nullptr;
}

bool Gizmo::isSelected(void) const {
	return m_selectedArrow != -1;
}

int Gizmo::getSelectedIndex(void) const {
	return m_selectedArrow;
}
