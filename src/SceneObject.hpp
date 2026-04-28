#pragma once
#include <filesystem>
#include <vector>

#include "Shader.hpp"
#include "glm/glm.hpp"

class Camera;
class Model;
struct LightProperties;

/**
 * @brief Selection effect to apply to a selected object
 */
struct Selection
{
	Shader color;
	float scale;
};

/**
 * @brief Corresponds to an object render into the scene
 */
class SceneObject
{
   protected:
	/**
	 * @brief Represents the 3D transformation of the object
     */
	struct transformation
	{
		glm::vec3 position;	 ///< The location
		glm::vec3 scale;	 ///< The scale
	} m_transformation;		 ///< The transformation of the object

   public:
	static Shader pick;	 ///< A static shader to represent the picking version of the objects

	/**
	 * @brief Default constructor
	 */
	SceneObject(void);

	/**
	 * @brief Constructor
	 * @param[in] geometry The model of the object to render
	 * @param[in] shader The shader of the objet to render
	 */
	SceneObject(Model* geometry, Shader* shader);

	/**
	 * @brief Load the model and compile the shader associated
	 */
	void load(void);

	/**
	 * @brief Returns the object's model
	 * @return The model
	 */
	Model* getModel(void) const;

	/**
	 * @brief Returs the object's position
	 * @return The position
	 */
	glm::vec3 getPosition(void) const;

	/**
	 * @brief Set the name of the object
	 * @param[in] name The new name
	 */
	void setName(const std::string& name);

	/**
	 * @brief Return the name of the object 
	 * @return The name
	 */
	const std::string& getName(void) const;

	/**
	 * @brief Render the scene object
	 * @param[in] camera The camera from which render the scene
	 * @param[in] lights All lights used in the scene for the render
	 */
	virtual void render(const Camera& camera, const std::vector<LightProperties*>& lights) const;

	/**
	 * @brief Render the selected version of the object
	 * @param[in] camera The camera from which to make the render
	 * @param[in] lights All the lights illuminating the scene
	 * @param[in] selection The selection effect to apply to the object
	 */
	void renderSelected(const Camera& camera, const std::vector<LightProperties*>& lights, const Selection& selection) const;

	/**
	 * @brief Render the scene object with its unique color for the picking operation
	 * @param[in] camera The camera from which render the scene
	 */
	virtual void renderPicking(const Camera& camera) const;

	/**
	 * @brief Tells if a color matches the object's one
	 * @param[in] id The id to test
	 * @return The result of the test
	 */
	virtual bool isId(const glm::ivec3& id) const;

	/**
	 * @brief Translate the object in the scene
	 * @param[in] translation The direction for the object to be translated to
	 */
	virtual void translate(const glm::vec3& translation);

	/**
	 * @brief Move the object to a specified location in the scene
	 * @param[in] position The new position
	 */
	void setPosition(const glm::vec3& position);

	/**
	 * @brief Scale the object
	 * @param[in] scale A float representing the percentage of the scale to apply
	 */
	void scale(float scale);

	/**
	 * @brief Scale the object
	 * @param[in] scale The vector representation of the scaling to apply
	 */
	void scale(glm::vec3 scale);

   protected:
	/**
	 * @brief Internal rendering function
	 * @param[in] camera The camera from which render the scene
	 * @param[in] lights All lights used in the scene for the render
	 * @param[in] model The model to render
	 * @param[in] shader The shader used on the model
	 * @param[in] transformation The transformation to apply to the object
     */
	void _render(const Camera& camera, const std::vector<LightProperties*>& lights, const Model& model, const Shader& shader,
				 const transformation& transformation) const;

	/**
	 * @brief Internal rendering function for a selected object
	 * @param[in] camera The camera from which render the scene
	 * @param[in] lights All lights used in the scene for the render
	 * @param[in] model The model to render
	 * @param[in] shader The shader used on the model
	 * @param[in] selection The selection effect to apply to the object
     */
	void _renderSelected(const Camera& camera, const std::vector<LightProperties*>& lights, const Model& model, const Shader& shader,
						 const Selection& selection) const;

	/**
	 * @brief Internal rendering picking function
	 * @param[in] camera The camera from which render the scene
	 * @param[in] model The model to render
	 * @param[in] colorId The ID of the object
	 * @param[in] transformation The transformation to apply when rendering
     */
	void _renderPicking(const Camera& camera, const Model& model, const glm::vec3& colorId, const transformation& transformation) const;

	/**
	 * @brief Set up the lights required to render the scene
	 * @param[in] camera The camera from which the scene is taken from
	 * @param[in] lights The lights illuminating the scene
	 */
	void _setUpLights(const Camera& camera, const std::vector<LightProperties*>& lights) const;

	/**
	 * @brief Set the transformation matrix of the object's shader
	 * @param[in] camera The camera from which the scene is taken from
	 * @param[in] shader The shader used on the model
	 * @param[in] transformation The transformation to apply
	 */
	void _setTransformation(const Camera& camera, const Shader& shader, const transformation& transformation) const;

	/**
	 * @brief Get the color id
	 * @return The color id
	 */
	glm::vec3 _getColorId(void) const;

	Model* m_model;		   ///< The model of the object
	Shader* m_shd;		   ///< The shader of the object
	glm::ivec3 m_colorId;  ///< The ID of the object represented as a color
	std::string m_name;	   ///< The name, used mostly for debugging
	bool m_loaded;		   ///< Indicates if the sceneobject has already been loaded or not
};
