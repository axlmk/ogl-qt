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

using SpaceCoord = glm::vec3;

class Camera {
public:
	Camera(const SpaceCoord& position, float fov = 45.f);
	
	SpaceCoord	getPosition()	const;
	glm::vec2	getDirection()	const;
	float		getFov()		const;
	float		getNearPlan()	const;
	float		getFarPlan()	const;

	void setTarget(SpaceCoord targ0et);
	void setPosition(SpaceCoord pos);
	void addRotation(float xOffset, float yOffset);
	void walk(bool keyPressed[], qint64 deltaTime);
	void rotateAround(glm::vec2 lastPos, glm::vec2 currentPos);

	glm::mat4 getSpaceMat();

private:

    std::string err_msg;

	SpaceCoord m_direction;
	SpaceCoord m_position;
	SpaceCoord m_target;
	CameraProjection m_projection;
	float m_fov;
	float m_nearPlan;
	float m_farPlan;
	float m_pitch;
	float m_yaw;
};
