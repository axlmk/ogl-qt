#pragma once

#include <memory>

#include "Shader.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"
#include "SceneObject.hpp"

enum class Mouvement {
	Forward,
	Backward,
	Left,
	Right
};

class SceneManager {
public:
	SceneManager();
	void initializeScene();
	void walkCamera(Mouvement mouvement, bool active);
	std::vector<std::shared_ptr<SceneObject>> getSceneObjects();
	void compileShaders();
	void loadSceneObjects();
	std::shared_ptr<Camera> getCamera();

private:

	bool m_cameraDirection[4];
	std::vector<std::shared_ptr<Geometry>> m_geometries;
	std::vector<std::shared_ptr<Shader>> m_shaders;
	std::vector<std::shared_ptr<SceneObject>> m_sceneObjects;
	std::shared_ptr<Camera> m_camera;
};
