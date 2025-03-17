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
	refCube->setNormals();
	m_geometries.push_back(std::unique_ptr<Geometry>(std::move(refCube)));

	Geometry* refCube2 = new Geometry(GeometryType::Cube, 0.5);
	refCube2->setNormals();
	refCube2->translate(0, 0, -4);
	m_geometries.push_back(std::unique_ptr<Geometry>(std::move(refCube2)));

	Geometry* refCube3 = new Geometry(GeometryType::Cube, 0.5);
	refCube3->setNormals();
	refCube3->translate(0, 0, -2);
	m_geometries.push_back(std::unique_ptr<Geometry>(std::move(refCube3)));

	Geometry* lightCube = new Geometry(GeometryType::Cube, 0.3);
	lightCube->translate(0.1, 2.0, 0.2);
	lightCube->setPivot(0.15, 0.15, 0.15);
	m_geometries.push_back(std::unique_ptr<Geometry>(std::move(lightCube)));

	Geometry* lightCube2 = new Geometry(GeometryType::Cube, 0.3);
	lightCube2->translate(0, 1.0, -4);
	m_geometries.push_back(std::unique_ptr<Geometry>(std::move(lightCube2)));

	Geometry* lightCube3 = new Geometry(GeometryType::Cube, 0.3);
	lightCube3->translate(0, -1, 0);
	m_geometries.push_back(std::unique_ptr<Geometry>(std::move(lightCube3)));

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
	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[1].get(), m_shaders[0].get()));
	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[2].get(), m_shaders[0].get()));
	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[3].get(), m_shaders[1].get(), SceneObjectType::Light));
	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[4].get(), m_shaders[1].get(), SceneObjectType::Light));
	m_sceneObjects.push_back(std::make_unique<SceneObject>(m_geometries[5].get(), m_shaders[1].get(), SceneObjectType::Light));

	m_lights.push_back(*m_sceneObjects[3]);
	m_lights[0].get().setSpotLight({0, -1, 0}, 12, 14);

	m_lights.push_back(*m_sceneObjects[4]);
	m_lights[1].get().setPointLight(0.35, 0.44);

	m_lights.push_back(*m_sceneObjects[5]);
	m_lights[2].get().setDirectionalLight({1.0, 1.0, 1.0});
}



void scene::renderLoop(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime)
{
	static unsigned int i = 0;
	static std::string smoothDT = "";

	for (auto& sceneObject : m_sceneObjects) {
		m_camera->move(inputsBeingPressed, deltaTime);
		//m_manager->getGeometries()[1]->rotate(i * 0.01, 0, 1, 0);
		sceneObject->render(m_camera.get(), getLights());
	}

	for (auto& hud : m_huds) {
		if (!(i % 4))
			smoothDT = std::to_string(int(1000 / deltaTime));
		hud->RenderText(smoothDT, 1, 1, { 255, 255, 255 }, 0.5);
	}
	i++;
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



std::vector<std::unique_ptr<Shader>>& scene::getShaders() {
	return m_shaders;
}

std::unique_ptr<Camera>& scene::getCamera()
{
	return m_camera;
}

std::vector<std::unique_ptr<Geometry>>& scene::getGeometries() {
	return m_geometries;
}

std::vector<std::reference_wrapper<SceneObject>>& scene::getLights()
{
	return m_lights;
}