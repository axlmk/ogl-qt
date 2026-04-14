#pragma once

#include <memory>

#include "LightProperties.hpp"
#include "SceneObject.hpp"
#include "glm/glm.hpp"

class LightObject : public SceneObject
{
   public:
	LightObject() = default;
	LightObject(Model* geometry, Shader* shader);

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
	void setPointLight(float linear, float quadratic);
	void setSpotLight(glm::vec3 direction, float cutoff, float outerCutoff = 0.0);

   private:
	LightProperties m_lightProperties;
};