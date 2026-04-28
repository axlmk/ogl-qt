#pragma once

#include <QtTypes>
#include <memory>
#include <unordered_map>

#include "Camera.hpp"
#include "Gizmo.hpp"
#include "HUD.hpp"
#include "LightObject.hpp"
#include "Model.hpp"
#include "PickingTexture.hpp"
#include "SceneObject.hpp"
#include "Shader.hpp"
#include "glm/glm.hpp"

#define MAX_LIGHT_NUMBER 10

struct LightProperties;

/**
 * @brief Handles the logic of the 3D scene
 */
class Scene
{
   public:
	/**
	 * @brief Default constructor
	 */
	Scene(void);

	/**
	 * @brief Returns the camera used by the scene
	 * @return The camera
	 */
	Camera& getCamera(void);

	/**
	 * @brief The mouse is clicked there we might try to select an object within the scene
	 * @param[in] mouseCoords The coordinates where the mouse is clicking
	 */
	void enablePicking(glm::ivec2 mouseCoords);

	/**
	 * @brief Disable the picking state of the scene
	 */
	void disablePicking(void);

	/**
	 * @brief Initialize the 3D scene
	 * @param[in] viewportWidth The width of the 3D scene
	 * @param[in] viewportHeight The height of the 3D scene
	 */
	void initializeScene(int viewportWidth, int viewportHeight);

	/**
	 * @brief Main loop of the application. Render the entire scene every time it's called
	 * @param[in] deltaTime The time difference between two renders
	 */
	void renderLoop(long long deltaTime);

	/**
	 * @brief Make the camera focus on the selected object
	 */
	void focusCameraOnSelectedObject(void);

	/**
	 * @brief Try to translate the selected object
	 * @param[in] mouseDiff The 2D vector describing the mouse mouvement between two renders
	 */
	void tryMoveObject(const glm::ivec2& mouseDiff);

	/**
	 * @brief Updates the viewport after a resizing of its window
	 * @param[in] width The new width of the window
	 * @param[in] height The new height of the window
	 */
	void updateViewport(int width, int height);

	/**
	 * @brief Add an object to the list of the scene's renderable
	 * @param[in] model The model of a new object
	 * @param[in] shader The shader of a new object
	 * @param[in] name The name of a new object
	 */
	void addObjectToRenderables(Model* model, Shader* shader, const std::string& name);

	/**
	 * @brief Add an object to the list of the scene's renderable
	 * @param[in] model The model of a new light
	 * @param[in] shader The shader of a new light
	 * @param[in] light The type of the light 
	 */
	void addLightToRenderables(Model* model, Shader* shader, LightProperties::LightType light);

	/**
	 * @brief Returns the select object coordinates through a string
	 * @return The coordinates of the select object as a string
	 */
	std::string getSelectedObjectCoordinateStr(void) const;

   private:
	/**
	 * @brief Do the picking phase, which means a special render of the image is made to determine even an object has been clicked on
	 */
	void _picking(void);

	std::vector<std::unique_ptr<HUD>> m_huds;					  ///< All the HUDs present in the scene
	std::vector<std::unique_ptr<SceneObject>> m_renderedObjects;  ///< All the objects present in the scene
	std::vector<LightProperties*> m_lights;						  ///< All the lights that illuminate the scene

	Camera m_camera;			  ///< The point of view of the user in the screen
	PickingTexture m_pickingTex;  ///< The texture used to defer the render of the picking phase
	Gizmo m_gizmo;				  ///< Gizmo places at the selected object's location

	SceneObject* m_selectedObject;	/// A reference to the selected object
	Selection m_selection;			///< The selection applied to selected objects
	bool m_isPicking = false;		///< Indicates if the user is trying to pick an object

	glm::ivec2 m_mouseCoords;  ///< The coordinates of the mouse during the render

	uint m_numberOfCreatedObjects;	///< Count the number of objects created throughout the project

	constexpr static float m_translatingUpFactor = 0.002f;	///< A factor of multiplication to translate the object up and down
	constexpr static float m_translatingXFactor = -0.002f;	///< A factor of multiplication to translate the object right and left
	constexpr static float m_translatingZFactor = 0.002f;	///< A factor of multiplication to translate the object front and back
};
