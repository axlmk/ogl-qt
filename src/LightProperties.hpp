#pragma once

#include "glm/glm.hpp"

struct LightProperties
{
	enum LightType { Spot, Point, Directional } type;
	/**
	 * @brief Default construct a LightProperties
	 */
	LightProperties();

	/**
	 * @brief Default constructor for specific type of light
	 * @param[in] typeLight The type of the light
	 */
	LightProperties(LightType typeLight);

	glm::vec3 position;
	float linear;
	float quadratic;
	glm::vec3 direction;
	float cutoff;
	float outerCutoff;
};