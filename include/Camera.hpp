#pragma once

#include <string>
#include <exception>
#include <QDebug>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Utils.hpp>

using SpaceCoord = glm::vec3;

class Camera {
public:
	Camera(const SpaceCoord& position, float fov = 45.f);
	
	SpaceCoord	getPosition()	const;
	glm::vec2	getDirection()	const;
	float		getFov()		const;
	float		getNearPlan()	const;
	float		getFarPlan()	const;

	void setPosition(SpaceCoord pos);
	void lookAround(float xOffset, float yOffset);
	void walk(bool keyPressed[], qint64 deltaTime);

	glm::mat4 getSpaceMat();

private:

    std::string err_msg;

	SpaceCoord m_direction;
	SpaceCoord m_position;
	float m_fov;
	float m_nearPlan;
	float m_farPlan;
	float m_pitch;
	float m_yaw;
};
