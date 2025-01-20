#include "Camera.hpp"


Camera::Camera(const SpaceCoord &position, float fov, CameraProjection projection, CameraType type): m_position{position}, m_projection{projection}, m_fov{fov}, m_nearPlan{0.1f}, m_farPlan{100.f}, m_rotation{glm::vec4(0.0f, 1.f, 0.f, 0.f)}, m_type{ type } {
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

void Camera::setType(CameraType type) {
	m_type = type;
}

void Camera::setTarget(SpaceCoord target) {
	if(m_type != CameraType::LookAt) {
		m_err_msg = "A target can't be specified when Camera type isn't LookAt";
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}
	m_target = target;
}

void Camera::setPosition(SpaceCoord pos) {
	m_position = pos;
}

glm::mat4 Camera::getSpaceMat() const {
    glm::mat4 view;
	switch(m_type) {
		case CameraType::LookAt:

			break;
		case CameraType::FirstPerson:
			glm::translate(view, m_position);
			view = glm::rotate(view, m_rotation.x, {m_rotation.y, m_rotation.z, m_rotation.w});
			break;
		default:
			m_err_msg == "Camera's type unrecognized";
			qCritical() << m_err_msg;
			throw std::invalid_argument(m_err_msg);
			break;
	}
}