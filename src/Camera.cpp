#include "Camera.hpp"


Camera::Camera(const SpaceCoord& position, float fov, CameraType type) : 
	m_position{ position },
	m_projection{ CameraProjection::Perspective },
	m_fov{ fov }, m_nearPlan{ 0.1f },
	m_farPlan{ 100.f },
	m_direction{ 0.0f, 0.0f, -1.0f },
	m_type{ type },
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

void Camera::setType(CameraType type) {
	m_type = type;
}

//void Camera::setTarget(SpaceCoord target) {
//	if(m_type != CameraType::LookAt) {
//		std::string err_msg = "A target can't be specified when Camera type isn't LookAt";
//		qCritical() << err_msg;
//		throw std::invalid_argument(err_msg);
//	}
//	m_target = target;
//}

void Camera::setPosition(SpaceCoord pos) {
	m_position = pos;
}



void Camera::addRotation(float xOffset, float yOffset) {
	if(m_type != CameraType::FirstPerson) {
		std::string err_msg = "Rotation can only modified while being in FirstPerson mode";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

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
	switch(m_type) {
		case CameraType::LookAt:
			view = glm::lookAt(m_position, m_direction, { 0, 1., 0 });
			break;
		case CameraType::FirstPerson:
			view = glm::lookAt(m_position, m_direction + m_position, { 0, 1., 0 });
			break;
		default:
			std::string err_msg = "Camera's type unrecognized";
			qCritical() << err_msg;
			throw std::invalid_argument(err_msg);
			break;
	}
	return view;
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