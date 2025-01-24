#include <SceneManager.hpp>


SceneManager::SceneManager() :
	m_cameraDirection{ false, false, false, false } {
}



void SceneManager::initializeScene() {

	// Camera

	auto mainCam{ std::make_shared<Camera>(SpaceCoord(0., 0., 2.)) };
	m_camera = mainCam;

	// Cubes

	auto texturedCube{ std::make_shared<Geometry>(GeometryType::Cube, .5) };
	texturedCube->setPivot(.75, .25, -.25);
	m_geometries.push_back(texturedCube);

	auto faceCube{ std::make_shared<Geometry>(GeometryType::Cube, .5) };
	faceCube->setPivot(.25, .25, -.25);
	faceCube->rotate(45, 1., 0.5);
	m_geometries.push_back(faceCube);
	
	// Shaders

	auto textShader{ std::make_shared<Shader>(ShaderType::Texture) };
	textShader->setTexture("textures/container.jpg");
	m_shaders.push_back(textShader);

	auto faceShader{ std::make_shared<Shader>(ShaderType::Texture) };
	faceShader->setTexture("textures/awesomeface.png");
	m_shaders.push_back(faceShader);
	
	// Scene creation

	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[0], m_shaders[0]));
	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[1], m_shaders[1]));
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



std::vector<std::shared_ptr<SceneObject>> SceneManager::getSceneObjects() {
	return m_sceneObjects;
}


void SceneManager::compileShaders() {

	for(auto &shader : m_shaders) {
		shader->compile();
	}
}

void SceneManager::loadSceneObjects() {

	for(auto &object : m_sceneObjects) {
		object->generateRender();
	}
}

std::shared_ptr<Camera> SceneManager::getCamera() {
	return m_camera;
}