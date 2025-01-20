#pragma once

#include <string>
#include <exception>
#include <QDebug>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraProjection {
	Perspective,
	Orthographic
};

enum class CameraType {
	LookAt,
	FirstPerson
};

using SpaceCoord = glm::vec3;

class Camera {
public:
	Camera(const SpaceCoord& position, float fov = 45.f, CameraProjection projection = CameraProjection::Perspective, CameraType type = CameraType::FirstPerson);
	SpaceCoord getPosition() const;
	glm::vec4 getRotation() const;
	float getFov() const;
	float getNearPlan() const;
	float getFarPlan() const;

	void setType(CameraType type);
	void setTarget(SpaceCoord target);
	void setPosition(SpaceCoord pos);

	glm::mat4 getSpaceMat() const;

private:

    std::string m_err_msg;
	glm::vec4 m_rotation;
	SpaceCoord m_position;
	SpaceCoord m_target;
	CameraProjection m_projection;
	CameraType m_type;
	float m_fov;
	float m_nearPlan;
	float m_farPlan;
};
