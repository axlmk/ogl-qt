#include "gizmo.hpp"

Gizmo::Gizmo(std::unique_ptr<SceneObject> red, std::unique_ptr<SceneObject> green, std::unique_ptr<SceneObject> blue)
	: m_selectedArrow{-1}, m_arrows{std::move(red), std::move(green), std::move(blue)}
{}

void Gizmo::render(const Camera& camera, const std::vector<std::reference_wrapper<SceneObject>>& lights) const
{

	const static float threshold = 0.25;
	const glm::vec3 A{0., 0., 1.};
	const glm::vec3 C{1., 0., 0.};
	const glm::vec3 D{0., 1., 0.};
	const auto B = camera.getDirection();
	const auto xCos = (glm::dot(A, B)) / (glm::length(A) * glm::length(B));
	const auto zCos = (glm::dot(C, B)) / (glm::length(C) * glm::length(B));
	const auto YCos = (glm::dot(D, B)) / (glm::length(D) * glm::length(B));

	const static float scalingFactor = 0.1f;

	if (std::abs(xCos) > threshold || std::abs(YCos) > threshold)
	{
		const auto lenCamArrow = glm::length(camera.getPosition() - m_arrows[0]->getPosition());
		m_arrows[0]->getModel()->scale(lenCamArrow * scalingFactor);
		m_arrows[0]->render(camera, lights);
	}

	const auto lenCamArrow = glm::length(camera.getPosition() - m_arrows[1]->getPosition());
	m_arrows[1]->getModel()->scale(lenCamArrow * scalingFactor);
	m_arrows[1]->render(camera, lights);

	if (std::abs(zCos) > threshold || std::abs(YCos) > threshold)
	{
		const auto lenCamArrow = glm::length(camera.getPosition() - m_arrows[2]->getPosition());
		m_arrows[2]->getModel()->scale(lenCamArrow * scalingFactor);
		m_arrows[2]->render(camera, lights);
	}
}

void Gizmo::setPosition(const glm::vec3& position)
{
	m_arrows[0]->getModel()->setPosition(position);
	m_arrows[1]->getModel()->setPosition(position);
	m_arrows[2]->getModel()->setPosition(position);
}

void Gizmo::renderPicking(const Camera& camera)
{
	const static float threshold = 0.25;
	const glm::vec3 A{0., 0., 1.};
	const glm::vec3 C{1., 0., 0.};
	const glm::vec3 D{0., 1., 0.};
	const auto B = camera.getDirection();
	const auto xCos = (glm::dot(A, B)) / (glm::length(A) * glm::length(B));
	const auto zCos = (glm::dot(C, B)) / (glm::length(C) * glm::length(B));
	const auto YCos = (glm::dot(D, B)) / (glm::length(D) * glm::length(B));

	const static float scalingFactor = 0.1f;

	if (std::abs(xCos) > threshold || std::abs(YCos) > threshold)
	{
		const auto lenCamArrow = glm::length(camera.getPosition() - m_arrows[0]->getPosition());
		m_arrows[0]->getModel()->scale(lenCamArrow * scalingFactor);
		m_arrows[0]->renderPicking(camera);
	}

	const auto lenCamArrow = glm::length(camera.getPosition() - m_arrows[1]->getPosition());
	m_arrows[1]->getModel()->scale(lenCamArrow * scalingFactor);
	m_arrows[1]->renderPicking(camera);

	if (std::abs(zCos) > threshold || std::abs(YCos) > threshold)
	{
		const auto lenCamArrow = glm::length(camera.getPosition() - m_arrows[2]->getPosition());
		m_arrows[2]->getModel()->scale(lenCamArrow * scalingFactor);
		m_arrows[2]->renderPicking(camera);
	}
}

bool Gizmo::isId(const glm::ivec3 id)
{
	if (m_arrows[0]->isId(id))
	{
		m_selectedArrow = 0;
	}
	else if (m_arrows[1]->isId(id))
	{
		m_selectedArrow = 1;
	}
	else if (m_arrows[2]->isId(id))
	{
		m_selectedArrow = 2;
	}
	else
	{
		m_selectedArrow = -1;
		return false;
	}
	return true;
}

SceneObject* Gizmo::getSelectedArrow(void) const
{
	if (m_selectedArrow >= 0 && m_selectedArrow < 3)
	{
		return m_arrows[m_selectedArrow].get();
	}
	return nullptr;
}

bool Gizmo::isSelected(void) const
{
	return m_selectedArrow != -1;
}

int Gizmo::getSelectedIndex(void) const
{
	return m_selectedArrow;
}
