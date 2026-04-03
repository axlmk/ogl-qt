#include <memory>

#include "glm/glm.hpp"

enum LightType { Spot, Point, Directional };

class SceneObject;

struct LightProperties
{
	LightType type;
	float linear;
	float quadratic;
	glm::vec3 direction;
	float cutoff;
	float outerCutoff;
};

class Light
{
   public:
	explicit Light();

	LightProperties getProperties(void) const;
	glm::vec3 getPosition(void) const;

	void setDirectionalLight(glm::vec3 direction);
	void setPointLight(float linear, float quadratic);
	void setSpotLight(glm::vec3 direction, float cutoff, float outerCutoff = 0.0);

   private:
	LightProperties m_lightProperties;
	std::unique_ptr<SceneObject> m_sceneObject;
};