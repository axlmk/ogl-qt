#pragma once

#include <memory>

#include "Shader.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"
#include "SceneObject.hpp"

// Forward declaration
class SceneViewer;


enum class Mouvement {
	Forward,
	Backward,
	Left,
	Right
};

class SceneManager {
public:
	SceneManager();
	std::vector<std::unique_ptr<SceneObject>>& getSceneObjects();
	std::vector<std::unique_ptr<Shader>>& getShaders();
	std::vector<std::unique_ptr<Geometry>>& getGeometries();
	void setSceneViewer(SceneViewer* sceneViewer);
	std::unique_ptr<Camera>& getCamera();
	
	void initializeScene();
	void walkCamera(Mouvement mouvement, bool active);

private:

	bool m_cameraDirection[4];
	std::vector<std::unique_ptr<Geometry>> m_geometries;
	std::vector<std::unique_ptr<Shader>> m_shaders;
	std::vector<std::unique_ptr<SceneObject>> m_sceneObjects;
	std::unique_ptr<Camera> m_camera;
	SceneViewer* m_viewer;
};
