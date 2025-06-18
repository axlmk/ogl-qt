#pragma once

#include <memory>

#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "SceneObject.hpp"
#include "HUD.hpp"
#include "PickingTexture.hpp"

// Forward declaration
class SceneViewer;

enum class Mouvement {
	Forward,
	Backward,
	Left,
	Right
};

class scene
{
public:
	scene();

	std::unique_ptr<Camera>& getCamera();
	std::vector<std::unique_ptr<Model>>& getModels();
	std::vector<std::unique_ptr<Shader>>& getShaders();
	std::vector<std::reference_wrapper<SceneObject>>	getLights();

	void tryToSelect(glm::ivec2 mouseCoords, int viewportWidth, int viewportHeight);

	void setSceneViewer(SceneViewer* sceneViewer);

	void initializeScene();
	void renderLoop(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime);
	void walkCamera(Mouvement mouvsement, bool active);

	bool m_isPicking = false;

private:

	void picking();

	bool m_cameraDirection[4];
	std::vector<std::unique_ptr<HUD>>					m_huds;
	std::vector<std::unique_ptr<Model>>					m_models;
	std::vector<std::unique_ptr<Shader>>				m_shaders;
	std::vector<std::unique_ptr<SceneObject>>			m_sceneObjects;
	std::vector<std::reference_wrapper<SceneObject>>	m_lights;

	SceneObject* m_selectedObject;
	std::unique_ptr<PickingTexture> m_pickingTex = nullptr;
	glm::ivec2 m_mouseCoords;

	std::unique_ptr<Camera> m_camera;
	SceneViewer* m_viewer;
};
