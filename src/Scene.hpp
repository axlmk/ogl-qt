#pragma once

#include <QtTypes>
#include <memory>
#include <unordered_map>

#include "Camera.hpp"
#include "Gizmo.hpp"
#include "HUD.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "PickingTexture.hpp"
#include "SceneObject.hpp"
#include "Shader.hpp"
#include "glm/glm.hpp"

class SceneViewer;

enum class Mouvement { Forward, Backward, Left, Right };

class scene
{
   public:
	scene();

	std::unique_ptr<Camera>& getCamera();
	std::vector<std::unique_ptr<Model>>& getModels();
	std::vector<std::unique_ptr<Shader>>& getShaders();
	std::vector<std::reference_wrapper<SceneObject>> getLights();

	void tryToSelect(glm::ivec2 mouseCoords, int viewportWidth, int viewportHeight);

	void setSceneViewer(SceneViewer* sceneViewer);

	void initializeScene();
	void renderLoop(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime);
	void walkCamera(Mouvement mouvsement, bool active);
	void focusCameraOnSelectedObject(void);
	void tryMoveObject(const glm::ivec2& mouseDiff);

	std::string getSelectedObjectCoordinateStr(void) const;

	bool m_isPicking = false;

   private:
	void picking();

	std::vector<std::unique_ptr<HUD>> m_huds;
	std::vector<std::unique_ptr<Model>> m_models;
	std::vector<std::unique_ptr<Shader>> m_shaders;
	std::vector<std::unique_ptr<SceneObject>> m_sceneObjects;
	std::vector<Light> m_lights;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<PickingTexture> m_pickingTex = nullptr;
	std::unique_ptr<Gizmo> m_gizmo;

	SceneObject* m_selectedObject;

	bool m_cameraDirection[4];
	glm::ivec2 m_mouseCoords;
	SceneViewer* m_viewer;
};
