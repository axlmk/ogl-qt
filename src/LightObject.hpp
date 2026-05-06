#pragma once

#include <memory>

#include "LightProperties.hpp"
#include "SceneObject.hpp"
#include "glm/glm.hpp"

/**
 * @brief Represents a light that can be added onto the scene
 */
class LightObject : public SceneObject
{
   public:
	/**
	 * @brief Default constructor
	 */
	LightObject() = default;

	/**
	 * @brief Constructor
	 * @param[in] geometry The model used
	 * @param[in] shader The shader used
	 */
	LightObject(Model* geometry, Shader* shader);

	/**
	 * @brief Render the scene object
	 * @param[in] camera The camera from which render the scene
	 * @param[in] lights All lights used in the scene for the render
	 */
	void render(const Camera& camera, const std::vector<LightProperties*>& lights) const override;

	/**
	 * @brief Translate the model and the light properties at the same time
	 * @param[in] translation The direction for the object to be translated to
	 */
	void translate(const glm::vec3& translation) override;

	/**
	 * @brief Return the light properties of the light object
	 * @return The light properties
	 */
	LightProperties* getLightProperties(void);

	/**
	 * @brief Set up the directional light parameters, therefore making the light directionnal
	 * @param[in] direction The direction of the directionnal light
	 */
	void setDirectionalLight(glm::vec3 direction);

	/**
	 * @brief Set up the point light parameters, therefore making a point light
	 * @param[in] linear Linear decrease of the light
	 * @param[in] quadratic Quadratic decrease of the light
	 * @note 
	 */
	void setPointLight(float linear, float quadratic);

	/**
	 * @brief Set up the spot light parameters, therefore making the a spot light
	 * @param[in] direction The direction to which the light is pointing to
	 * @param[in] cutoff The inner radius of the spotlight
	 * @param[in] outerCutoff The ouder radius of the spotlight (creates a a smooth transition between light and shadow)
	 */
	void setSpotLight(glm::vec3 direction, float cutoff, float outerCutoff = 0.0);

	/**
	 * @brief Set the direction of the light, only for directional and spot light
	 * @param[in] direction The new direction of the light
	 */
	void setLightDirection(glm::vec3 direction);

   private:
	LightProperties m_lightProperties;	///< The rendering properties of the light represent by this lightobject
};