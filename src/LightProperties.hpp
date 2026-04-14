#pragma once

#include "glm/glm.hpp"

struct LightProperties
{
	/**
	 * @brief Default construct a LightProperties
	 */
	LightProperties();
	glm::vec3 position;
	enum LightType { Spot, Point, Directional } type;
	float linear;
	float quadratic;
	glm::vec3 direction;
	float cutoff;
	float outerCutoff;
};