#pragma once

#include <Utils.hpp>
#include <cmath>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

/**
 * @brief Describes the type of projection used by the camera
 */
enum class CameraProjection {
	Perspective,  ///< Persepective camera
	Orthographic  ///< Orthographic camera
};

/**
 * @brief Represents a point of view of the scene
 * @note The camera has a position as well as a target. When rotating around, the camera is always rotation around this target
 * When panning, the camera is translating its position and its target's
 * When zooming, it just gets more or less closer to the target
 */
class Camera
{
   public:
	/**
	 * @brief Default constructor
	 * @param[in] position Position of the camera in the scene
	 * @param[in] fov The FOV of the camera
	 */
	Camera(const glm::vec3& position, float fov = 45.f);

	/**
	 * @brief Returns the FOV
	 * @return The FOV representes as a float
	 */
	float getFov(void) const;

	/**
	 * @brief Returns the far plan of the camera
	 * @return The far plan
	 */
	float getFarPlan(void) const;

	/**
	 * @brief Returns the near plan of the camera
	 * @return The near plan
	 */
	float getNearPlan(void) const;

	/**
	 * @brief Get the space matrix representing the camera
	 * @return The space matrix
	 */
	glm::mat4 getSpaceMat(void) const;

	/**
	 * @brief Get the position of the camera
	 * @return The position
	 */
	glm::vec3 getPosition(void) const;

	/**
	 * @brief Get the position of the camera as a string
	 * @return The position as a string
	 */
	std::string getPositionStr(void) const;

	/**
	 * @brief Get the direction the camera is looking to
	 * @return The direction
	 */
	glm::vec3 getDirection(void) const;

	/**
	 * @brief Get the target the camera is following
	 * @return The target position
	 */
	glm::vec3 getTarget(void) const;

	/**
	 * @brief Set the new position of the camera's target
	 * @param[in] target The new position of the target
	 */
	void setTarget(glm::vec3 target);

	/**
	 * @brief Set the new position of the camera
	 * @param[in] pos The new position
	 */
	void setPosition(glm::vec3 pos);

	/**
	 * @brief Rotates the camera around its target
	 * @param[in] lastPos Position of the mouse cursor during the previous render
	 * @param[in] currentPos Current position of the mouse cursor 
	 */
	void rotate(glm::vec2 lastPos, glm::vec2 currentPos);

	/**
	 * @brief Pans the camera
	 * @param[in] lastPos Position of the mouse cursor during the previous render
	 * @param[in] currentPos Current position of the mouse cursor 
	 */
	void pan(glm::vec2 lastPos, glm::vec2 currentPos);

	/**
	 * @brief Zoom onto the camera's target
	 * @param[in] lastPos Position of the mouse cursor during the previous render
	 * @param[in] currentPos Current position of the mouse cursor 
	 */
	void zoom(glm::vec2 lastPos, glm::vec2 currentPos);

   private:
	glm::vec3 m_position;  ///< The position of the camera
	glm::vec3 m_target;	   ///< The position of the camera's target
	glm::vec3 m_rightVec;  ///< A vector pointing to the right of the camera's relative direction
	glm::vec3 m_upVec;	   ///< A vector pointing to the top of the camera's relative direction

	CameraProjection m_projection;	///< The projection, can be perspective or orthogonal
	float m_fov;					///< The field of view
	float m_nearPlan;				///< The distance limit until objects are too close and can't be rendered
	float m_farPlan;				///< The distance limit until objects are too far and can't be rendered
};
