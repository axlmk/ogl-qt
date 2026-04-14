#pragma once

#include <Utils.hpp>
#include <cmath>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

enum class CameraProjection { Perspective, Orthographic };

using SpaceCoord = glm::vec3;

class Camera
{
   public:
	Camera(const SpaceCoord& position, float fov = 45.f);

	float getFov() const;
	float getFarPlan() const;
	float getNearPlan() const;
	glm::mat4 getSpaceMat() const;
	SpaceCoord getPosition() const;
	std::string getPositionStr(void) const;
	SpaceCoord getDirection() const;
	SpaceCoord getTarget(void) const;

	void setTarget(SpaceCoord targ0et);
	void setPosition(SpaceCoord pos);

	void move(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime);

	void rotate(glm::vec2 lastPos, glm::vec2 currentPos);
	void pan(glm::vec2 lastPos, glm::vec2 currentPos);
	void zoom(glm::vec2 lastPos, glm::vec2 currentPos);

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
