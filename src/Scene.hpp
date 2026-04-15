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

struct LightProperties;

enum class Mouvement { Forward, Backward, Left, Right };

class scene
{
   public:
	scene();

	//ajouter la selection aux objets et garder trace de quel objet est selectionne

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

	void initializeScene(int viewportWidth, int viewportHeight);
	void renderLoop(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime);
	void walkCamera(Mouvement mouvsement, bool active);
	void focusCameraOnSelectedObject(void);
	void tryMoveObject(const glm::ivec2& mouseDiff);

	/**
	 * @brief Updates the viewport after a resizing of its window
	 * @param[in] width The new width of the window
	 * @param[in] heigth The new height of the window
	 */
	void updateViewport(int width, int height);

	/**
	 * @brief Add an object to the list of the scene's renderable
	 * @param[in] renderable The object to add
	 */
	void addObjectToRenderables(SceneObject* renderable);

	std::string getSelectedObjectCoordinateStr(void) const;

   private:
	void _picking();

	std::vector<std::unique_ptr<HUD>> m_huds;	  ///< All the HUDs present in the scene
	std::vector<SceneObject*> m_renderedObjects;  ///< All the objects present in the scene
	std::vector<LightProperties*> m_lights;		  ///< All the lights that illuminate the scene

	Camera m_camera;
	PickingTexture m_pickingTex;
	Gizmo m_gizmo;	///< Gizmo places at the selected object's location

	SceneObject* m_selectedObject;

	Selection m_selection;	///< The selection applied to selected objects

	bool m_cameraDirection[4];
	glm::ivec2 m_mouseCoords;

	bool m_isPicking = false;  ///< Indicates if the user is trying to pick an object
};
