#include <Scene.hpp>
#include <SceneViewer.hpp>

scene::scene() :
	m_cameraDirection { false, false, false, false },
	m_viewer { nullptr } {
}



void scene::initializeScene()
{
	m_camera = { std::make_unique<Camera>(SpaceCoord(0.0, 0.0, 3.0)) };

	Geometry* refCube = new Geometry(GeometryType::Cube, 0.5);
	refCube->setPivot(0.25, 0.25, 0.25);
	refCube->setNormals();
	m_geometries.push_back(std::unique_ptr<Geometry>(std::move(refCube)));

	Geometry* lightCube = new Geometry(GeometryType::Cube, 0.3);
	lightCube->setPivot(0.15, 0.15, 0.15);
	lightCube->translate(0.7, 1.0, 0.8);
	m_geometries.push_back(std::unique_ptr<Geometry>(std::move(lightCube)));

	Shader* color = new Shader(ShaderType::Texture);
	color->addTexture("textures/container2.png");
	color->addTexture("textures/container2_specular.png");
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(color)));

	Shader* lightShd = new Shader(ShaderType::Light);
	lightShd->setLight();
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(lightShd)));

	HUD* hud = new HUD("arial");
	m_huds.push_back(std::unique_ptr<HUD>(std::move(hud)));

	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[0].get(), m_shaders[0].get()));
	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[1].get(), m_shaders[1].get(), SceneObjectType::Light));

	m_lights.push_back(*m_sceneObjects[1]);
	m_lights[0].get().setLightProperties(LightType::Directional, {-1, -1, -1});
}



void scene::walkCamera(Mouvement mouvement, bool active) {
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



std::vector<std::unique_ptr<SceneObject>>& scene::getSceneObjects() {
	return m_sceneObjects;
}


std::unique_ptr<Camera>& scene::getCamera() {
	return m_camera;
}

std::vector<std::unique_ptr<Shader>>& scene::getShaders() {
	return m_shaders;
}

std::vector<std::unique_ptr<Geometry>>& scene::getGeometries() {
	return m_geometries;
}

std::vector<std::unique_ptr<HUD>>& scene::getHUDs() {
	return m_huds;
}

std::vector<std::reference_wrapper<SceneObject>>& scene::getLights()
{
	return m_lights;
}