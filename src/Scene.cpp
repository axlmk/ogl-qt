#include <Scene.hpp>
#include <SceneViewer.hpp>

scene::scene() :
	m_cameraDirection { false, false, false, false },
	m_viewer { nullptr } {
}



void scene::initializeScene()
{
	m_camera = { std::make_unique<Camera>(SpaceCoord(0.0, 0.0, 3.0)) };

	Model *backpack_mdl = new Model("resources/models/backpack/backpack.obj");
	m_models.push_back(std::unique_ptr<Model>(std::move(backpack_mdl)));

	Model* light_mdl = new Model("resources/models/backpack/backpack.obj");
	m_models.push_back(std::unique_ptr<Model>(std::move(light_mdl)));

	Shader *backpack_shd = new Shader(ShaderType::Texture);
	backpack_shd->addTexture("resources/models/backpack/diffuse.jpg");
	backpack_shd->addTexture("resources/models/backpack/specular.jpg");
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(backpack_shd)));

	Shader *red = new Shader(ShaderType::Unicolor);
	red->setColor("#CC0000");
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(red)));

	Shader *light_shd = new Shader(ShaderType::Light);
	light_shd->setLight();
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(light_shd)));

	m_selection = { red, 1.04f };

	SceneObject *backpack = new SceneObject(backpack_mdl, backpack_shd, m_selection);
	backpack->debugName = "backpack";
	backpack->select();
	SceneObject *light = new SceneObject(light_mdl, light_shd, m_selection, SceneObjectType::Light);
	light->debugName = "light";

	m_sceneObjects.push_back(std::unique_ptr<SceneObject>(std::move(light)));
	m_sceneObjects.push_back(std::unique_ptr<SceneObject>(std::move(backpack)));

	m_lights.push_back(*m_sceneObjects[1]);
	m_lights[0].get().setDirectionalLight({1, 1, 1});
}



void scene::renderLoop(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime)
{
	static unsigned int i = 0;
	static std::string smoothDT = "";

	g_opengl.glEnable(GL_DEPTH_TEST);
	g_opengl.glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	g_opengl.glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (auto& sceneObject : m_sceneObjects)
	{
		m_camera->move(inputsBeingPressed, deltaTime);
		sceneObject->render(*m_camera, getLights());
	}

	for (auto& hud : m_huds)
	{
		if (!(i % 4))
		{
			smoothDT = std::to_string(int(1000 / deltaTime));
		}
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

std::vector<std::unique_ptr<Model>>& scene::getModels() {
	return m_models;
}

std::vector<std::reference_wrapper<SceneObject>>& scene::getLights()
{
	return m_lights;
}