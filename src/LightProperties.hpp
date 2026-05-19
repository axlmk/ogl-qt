#pragma once

#include "glm/glm.hpp"

/**
 * @brief Represents the physical properties used to simulate a light in the 3D scene
 */
struct LightProperties
{
	/**
	 * @brief The type of light
	 */
	enum LightType {
		Spot,		 ///< Spot light, like a flashlight
		Point,		 ///< Point light, like a bulb
		Directional	 ///< Directionnal light, like the sun
	} type;

	/**
	 * @brief Default construct a LightProperties
	 */
	LightProperties(void);

	/**
	 * @brief Default constructor for specific type of light
	 * @param[in] typeLight The type of the light
	 */
	LightProperties(LightType typeLight);

	/**
	 * @brief Set the inner cutoff angle for the light
	 * @param[in] cutoff The inner cutoff angle
	 */
	void setCutoff(float cutoff);

	/**
	 * @brief Set the outer cutoff angle for the light
	 * @param[in] outerCutoff The outer cutoff angle
	 */
	void setOuterCutoff(float outerCutoff);

	/**
	 * @brief Get the inner cutoff angle for the light
	 * @return The inner cutoff angle
	 */
	float getCutoff(void) const;

	/**
	 * @brief Get the outer cutoff angle for the light
	 * @return The outer cutoff angle
	 */
	float getOuterCutoff(void) const;

	glm::vec3 position;	  ///< The position
	float linear;		  ///< Point only, the linear decrease
	float quadratic;	  ///< Point only, the quadratic decrease
	glm::vec3 direction;  /// Directionnal or spot, the direction the light is pointing to
	float intensity;	  ///< The overall intensity of the light, used to easily modify the brightness

   private:
	float cutoff;		///< Spotlight only, the inner radius of the spotlight
	float outerCutoff;	///< Spotlight only, the ouder radius of the spotlight (creates a a smooth transition between light and shadow)
};