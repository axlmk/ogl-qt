#include <SceneManager.hpp>
#include <SceneViewer.hpp>


SceneManager::SceneManager() :
	m_cameraDirection{ false, false, false, false } {
}



void SceneManager::initializeScene() {

	// Camera

	m_camera = { std::make_unique<Camera>(SpaceCoord(0., 0., 2.)) };

	// Cubes

	Geometry* texturedCube = new Geometry(GeometryType::Cube, 0.5);
	texturedCube->setPivot(0.25, 0.25, 0.25);
	texturedCube->translate(-0.25);
	m_geometries.push_back(std::unique_ptr<Geometry>(texturedCube));

	Geometry* uniCube = new Geometry(GeometryType::Cube, 0.5);
	uniCube->setPivot(0.25, 0.25, 0.25);
	uniCube->translate(0.25);
	m_geometries.push_back(std::unique_ptr<Geometry>(uniCube));

	// Shaders

	Shader* face = new Shader(ShaderType::Texture);
	face->setTexture("textures/container.jpg");
	m_shaders.push_back(std::unique_ptr<Shader>(face));

	Shader* uni = new Shader(ShaderType::Unicolor);
	uni->setColor("#a4dc18");
	m_shaders.push_back(std::unique_ptr<Shader>(uni));

	// Scene creation

	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[0].get(), m_shaders[0].get()));
	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[1].get(), m_shaders[1].get()));
}



void SceneManager::walkCamera(Mouvement mouvement, bool active) {
	switch (mouvement) {
		case Mouvement::Forward:
			m_cameraDirection[0] = active;
			break;
		case Mouvement::Backward:
			m_cameraDirection[3] = active;
			break;
		case Mouvement::Left:
			m_cameraDirection[1] = active;
			break;
		case Mouvement::Right:
			m_cameraDirection[2] = active;
			break;
		default:
			break;
	}
}



std::vector<std::unique_ptr<SceneObject>>& SceneManager::getSceneObjects() {
	return m_sceneObjects;
}


std::unique_ptr<Camera>& SceneManager::getCamera() {
	return m_camera;
}

std::vector<std::unique_ptr<Shader>>& SceneManager::getShaders() {
	return m_shaders;
}

std::vector<std::unique_ptr<Geometry>>& SceneManager::getGeometries() {
	return m_geometries;
}