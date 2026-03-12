#include "Camera.hpp"

Camera::Camera(const SpaceCoord& position, float fov)
	: m_fov{fov},
	  m_upVec{0.0f, 1.0f, 0.0f},
	  m_target{0.0f, 0.0f, 0.0f},
	  m_farPlan{100.f},
	  m_nearPlan{0.1f},
	  m_rightVec{glm::cross(glm::normalize(position), {0.0, 1.0, 0.0})},
	  m_position{position},
	  m_projection{CameraProjection::Perspective}
{
	m_upVec = glm::cross(glm::normalize(position), m_rightVec);
}

SpaceCoord Camera::getPosition() const
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

void Camera::setPosition(SpaceCoord pos)
{
	m_position = pos;
}

void Camera::setTarget(SpaceCoord target)
{
	m_target = target;
}

SpaceCoord Camera::getDirection(void) const
{
	return m_position - m_target;
}

glm::mat4 Camera::getSpaceMat() const
{
	glm::mat4 view = glm::mat4(1.);
	return glm::lookAt(m_position, m_target, {0.0, 1.0, 0.0});
}

void Camera::move(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime)
{
	float speed = 2 * deltaTime * 0.001;
	SpaceCoord direction = glm::normalize(m_target - m_position);

	SpaceCoord forward = speed * direction;
	SpaceCoord right = speed * glm::normalize(glm::cross(direction, m_upVec));

	if (inputsBeingPressed["z"])
	{
		m_position += forward;
		m_target += forward;
	}
	if (inputsBeingPressed["q"])
	{
		m_position -= right;
		m_target -= right;
	}
	if (inputsBeingPressed["d"])
	{
		m_position += right;
		m_target += right;
	}
	if (inputsBeingPressed["s"])
	{
		m_position -= forward;
		m_target -= forward;
	}
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
	auto comparisonVector = SpaceCoord(tempDirection.x, 0.0, tempDirection.z);
	auto angleWithRightAxis = std::acos(glm::dot(tempDirection, comparisonVector) /
										(glm::length(comparisonVector) * glm::length(tempDirection)));

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
	SpaceCoord right = speed * m_rightVec;

	float deltaY = currentPos.y - lastPos.y;
	speed = deltaY * 0.002;
	SpaceCoord up = speed * m_upVec;

	m_position += right - up;
	m_target += right - up;
}

void Camera::zoom(glm::vec2 lastPos, glm::vec2 currentPos)
{
	float speed = (currentPos.x - lastPos.x) * 2 * 0.01;
	SpaceCoord mouvement = (m_position - m_target) * speed;

	if (glm::dot(m_position - m_target, m_position - m_target - mouvement) >= 0 &&
		glm::length(m_position - m_target - mouvement) > 0.1)
	{
		m_position -= mouvement;
	}
}

SpaceCoord Camera::getTarget(void) const
{
	return m_target;
}

std::string Camera::getPositionStr(void) const
{
	return "Camera: " + std::to_string(m_position.x) + "x " + std::to_string(m_position.y) + "y " +
		   std::to_string(m_position.z) + "z ";
}