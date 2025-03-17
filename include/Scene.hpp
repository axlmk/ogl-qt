#pragma once

#include <memory>

#include "Shader.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"
#include "SceneObject.hpp"
#include "HUD.hpp"

// Forward declaration
class SceneViewer;


enum class Mouvement {
	Forward,
	Backward,
	Left,
	Right
};

class scene {
public:
	scene();
	std::vector<std::unique_ptr<Shader>>&		getShaders();
	std::vector<std::unique_ptr<Geometry>>&		getGeometries();
	std::unique_ptr<Camera>&					getCamera();
	std::vector<std::reference_wrapper<SceneObject>>&					getLights();
	void setSceneViewer(SceneViewer* sceneViewer);
	
	void initializeScene();
	void renderLoop(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime);
	void walkCamera(Mouvement mouvement, bool active);

private:

	bool m_cameraDirection[4];
	std::vector<std::unique_ptr<Geometry>>				m_geometries;
	std::vector<std::unique_ptr<Shader>>				m_shaders;
	std::vector<std::reference_wrapper<SceneObject>>	m_lights;
	std::vector<std::unique_ptr<HUD>>					m_huds;
	std::vector<std::unique_ptr<SceneObject>>			m_sceneObjects;
	
	std::unique_ptr<Camera> m_camera;
	SceneViewer* m_viewer;
};
