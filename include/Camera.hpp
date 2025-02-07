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
	float		getFov()		const;
	float		getNearPlan()	const;
	float		getFarPlan()	const;

	void setTarget(SpaceCoord targ0et);
	void setPosition(SpaceCoord pos);

	void move(bool keyPressed[], qint64 deltaTime);

	void rotate(glm::vec2 lastPos, glm::vec2 currentPos);
	void pan(glm::vec2 lastPos, glm::vec2 currentPos);
	void zoom(glm::vec2 lastPos, glm::vec2 currentPos);

	glm::mat4 getSpaceMat();

private:

	SpaceCoord m_position;
	SpaceCoord m_target;
	SpaceCoord m_rightVec;
	SpaceCoord m_upVec;
	
	CameraProjection m_projection;
	float m_fov;
	float m_nearPlan;
	float m_farPlan;
};
