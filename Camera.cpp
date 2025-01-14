#include "Camera.hpp"


Camera::Camera(const SpaceCoord& position, float fov, CameraProjection projection) : m_position{ position }, m_projection{ projection }, m_fov{ fov }, m_nearPlan{ 0.1f }, m_farPlan{ 100.f }, m_rotation{ glm::vec4(0.0f, 1.f, 0.f, 0.f) } {
}

SpaceCoord Camera::getPosition() const {
	return m_position;
}

glm::vec4 Camera::getRotation() const {
	return  m_rotation;
}

float Camera::getFov() const {
	return m_fov;
}

float Camera::getNearPlan() const {
	return m_nearPlan;
}

float Camera::getFarPlan() const {
	return m_farPlan;
}