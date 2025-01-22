#pragma once

#include <string>
#include <exception>
#include <QDebug>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Utils.hpp>

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
	Camera(const SpaceCoord& position, float fov = 45.f, CameraType type = CameraType::FirstPerson);
	
	SpaceCoord	getPosition()	const;
	glm::vec2	getDirection()	const;
	float		getFov()		const;
	float		getNearPlan()	const;
	float		getFarPlan()	const;

	void setType(CameraType type);
	//void setTarget(SpaceCoord target);
	void setPosition(SpaceCoord pos);
	void addRotation(float xOffset, float yOffset);
	void walk(bool keyPressed[]);

	glm::mat4 getSpaceMat() const;

private:

    std::string m_err_msg;

	SpaceCoord m_direction;
	SpaceCoord m_position;
	//SpaceCoord m_target;
	CameraProjection m_projection;
	CameraType m_type;
	float m_fov;
	float m_nearPlan;
	float m_farPlan;
	float m_pitch;
	float m_yaw;
};
