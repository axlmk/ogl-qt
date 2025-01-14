#pragma once

#include <glm/glm.hpp>

enum class CameraProjection {
	Perspective,
	Orthographic
};

using SpaceCoord = glm::vec3;

class Camera {
public:
	Camera(const SpaceCoord& position, float fov = 45.f, CameraProjection projection = CameraProjection::Perspective);
	SpaceCoord getPosition() const;
	glm::vec4 getRotation() const;
	float getFov() const;
	float getNearPlan() const;
	float getFarPlan() const;

private:
	glm::vec4 m_rotation;
	SpaceCoord m_position;
	CameraProjection m_projection;
	float m_fov;
	float m_nearPlan;
	float m_farPlan;
};
