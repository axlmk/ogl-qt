#include "Camera.hpp"

Camera::Camera(const glm::vec3& position, float fov)
	: m_position{position},
	  m_target{0.0f, 0.0f, 0.0f},
	  m_rightVec{glm::cross(glm::normalize(position), {0.0, 1.0, 0.0})},
	  m_upVec{0.0f, 1.0f, 0.0f},
	  m_projection{CameraProjection::Perspective},
	  m_fov{fov},
	  m_nearPlan{0.1f},
	  m_farPlan{100.f}
{
	m_upVec = glm::cross(glm::normalize(position), m_rightVec);
}

glm::vec3 Camera::getPosition() const
{
	return m_position;
}

float Camera::getFov() const
{
	return m_fov;
}

float Camera::getNearPlan() const
{
	return m_nearPlan;
}

float Camera::getFarPlan() const
{
	return m_farPlan;
}

void Camera::setPosition(glm::vec3 pos)
{
	m_position = pos;
}

void Camera::setTarget(glm::vec3 target)
{
	m_target = target;
}

glm::vec3 Camera::getDirection(void) const
{
	return m_position - m_target;
}

glm::mat4 Camera::getSpaceMat() const
{
	return glm::lookAt(m_position, m_target, {0.0, 1.0, 0.0});
}

void Camera::rotate(glm::vec2 lastPos, glm::vec2 currentPos)
{
	float intensity = 0.05;
	float yConstraint = 0.15;
	auto direction = m_position - m_target;

	float deltaMouseX = currentPos.x - lastPos.x;
	float angleUpAxis = std::atan(deltaMouseX / glm::length(direction)) * intensity;

	float deltaMouseY = currentPos.y - lastPos.y;
	float angleRightAxis = std::atan(deltaMouseY / glm::length(direction)) * intensity;

	auto rightRotation = glm::rotate(glm::mat4(1.0), -angleUpAxis, {0.0, 1.0, 0.0});
	auto rotation = glm::rotate(rightRotation, angleRightAxis, m_rightVec);

	glm::vec3 tempDirection = rotation * glm::vec4(direction, 1.0);
	auto comparisonVector = glm::vec3(tempDirection.x, 0.0, tempDirection.z);
	auto angleWithRightAxis = std::acos(glm::dot(tempDirection, comparisonVector) / (glm::length(comparisonVector) * glm::length(tempDirection)));

	if (angleWithRightAxis < M_PI / 2 * (1 - yConstraint))
	{
		m_position = tempDirection + m_target;
		m_rightVec = rightRotation * glm::vec4(m_rightVec, 1.0);
		m_upVec = rotation * glm::vec4(m_upVec, 1.0);
	}
}

void Camera::pan(glm::vec2 lastPos, glm::vec2 currentPos)
{
	float deltaX = currentPos.x - lastPos.x;
	float speed = deltaX * 0.002;
	glm::vec3 right = speed * m_rightVec;

	float deltaY = currentPos.y - lastPos.y;
	speed = deltaY * 0.002;
	glm::vec3 up = speed * m_upVec;

	m_position += right - up;
	m_target += right - up;
}

void Camera::zoom(glm::vec2 lastPos, glm::vec2 currentPos)
{
	float speed = (currentPos.x - lastPos.x) * 2 * 0.01;
	glm::vec3 mouvement = (m_position - m_target) * speed;

	if (glm::dot(m_position - m_target, m_position - m_target - mouvement) >= 0 && glm::length(m_position - m_target - mouvement) > 0.1)
	{
		m_position -= mouvement;
	}
}

glm::vec3 Camera::getTarget(void) const
{
	return m_target;
}

std::string Camera::getPositionStr(void) const
{
	return "Camera: " + std::to_string(m_position.x) + "x " + std::to_string(m_position.y) + "y " + std::to_string(m_position.z) + "z ";
}