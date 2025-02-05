#include "Camera.hpp"


Camera::Camera(const SpaceCoord& position, float fov) : 
	m_position{ position },
	m_projection{ CameraProjection::Perspective },
	m_fov{ fov }, m_nearPlan{ 0.1f },
	m_farPlan{ 100.f },
	m_target { 0.0f, 0.0f, 0.0f },
	m_direction{ 0.0f, 0.0f, -1.0f },
	m_yaw{ -90.0f },
	m_pitch{ 0.0f } {
}

SpaceCoord Camera::getPosition() const {
	return m_position;
}

glm::vec2 Camera::getDirection() const {
	return  m_direction;
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


void Camera::setPosition(SpaceCoord pos) {
	m_position = pos;
}

void Camera::setTarget(SpaceCoord target) {
	/*float angle = std::atan((target.x - m_direction.x) / (target.z - m_direction.z + 1));

	glm::mat4 rotation = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.0, 1.0, 0.0));

	glm::vec4 rotated = rotation * glm::vec4(m_direction, 1.0f);*/


	m_target = target;
	m_direction = m_target - m_position;
}

void Camera::addRotation(float xOffset, float yOffset) {

	m_yaw += xOffset;
	m_pitch += yOffset;

	if(m_pitch >= 85)
		m_pitch = 85;
	else if(m_pitch <= -75)
		m_pitch = -75;

	SpaceCoord direction {
		cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
		sin(glm::radians(m_pitch)),
		sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
	};

	m_direction = glm::normalize(direction);
}



glm::mat4 Camera::getSpaceMat() {
    glm::mat4 view = glm::mat4(1.);
	m_position.y = 0.0f;
	return glm::lookAt(m_position, m_direction + m_position, { 0, 1., 0 });
}

void Camera::walk(bool keyPressed[], qint64 deltaTime) {
	float speed = 2 * deltaTime * 0.001;
	if (keyPressed[0])
		m_position += speed * m_direction;
	if (keyPressed[1])
		m_position -= speed * glm::normalize(glm::cross(m_direction, { 0.0, 1.0, 0.0 }));
	if (keyPressed[2])
		m_position += speed * glm::normalize(glm::cross(m_direction, { 0.0, 1.0, 0.0 }));
	if (keyPressed[3])
		m_position -= speed * m_direction;
}

void Camera::rotateAround(glm::vec2 lastPos, glm::vec2 currentPos) {

	float intensity = 0.1f;
	
	// X axis
	float deltaX = currentPos.x - lastPos.x;
	float angle = std::atan(deltaX / glm::length(m_direction)) * intensity;

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 1.0, 0.0));

	glm::vec4 rotated = rotation * glm::vec4(m_position, 1.0f);
	m_position.x = rotated.x + m_target.x;
	m_position.z = rotated.z + m_target.z;

	m_direction = m_target - m_position;
}
