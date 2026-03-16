#include <Scene.hpp>
#include <SceneViewer.hpp>

scene::scene() : m_selectedObject{nullptr}, m_cameraDirection{false, false, false, false}, m_viewer{nullptr} {}

void scene::initializeScene()
{
	m_camera = {std::make_unique<Camera>(SpaceCoord(0.0, 0.0, 3.0))};

	Model* backpack_mdl = new Model("resources/models/backpack/backpack2.obj");
	m_models.push_back(std::unique_ptr<Model>(std::move(backpack_mdl)));

	Shader* backpack_shd = new Shader(ShaderType::Texture);
	backpack_shd->addTexture("resources/models/backpack/diffuse.jpg");
	backpack_shd->addTexture("resources/models/backpack/specular.jpg");
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(backpack_shd)));

	Model* ground_mdl = new Model("resources/models/ground/ground.obj");
	m_models.push_back(std::unique_ptr<Model>(std::move(ground_mdl)));
	ground_mdl->scale(0.5);
	ground_mdl->setPosition({0, -2, 0});

	Shader* ground_shd = new Shader(ShaderType::Texture);
	ground_shd->addTexture("resources/models/ground/diffuse.png");
	ground_shd->addTexture("resources/models/ground/specular.png");
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(ground_shd)));

	// Gizmo

	Model* x_model = new Model("resources/models/gizmo/x.obj");
	m_models.push_back(std::unique_ptr<Model>(std::move(x_model)));
	Shader* x_shd = new Shader(ShaderType::Unicolor);
	x_shd->setColor("#FF0000");
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(x_shd)));
	std::unique_ptr<SceneObject> x = std::make_unique<SceneObject>(x_model, x_shd, SceneObjectType::Gizmo);
	x->debugName = "r";

	Model* y_model = new Model("resources/models/gizmo/y.obj");
	m_models.push_back(std::unique_ptr<Model>(std::move(y_model)));
	Shader* y_shd = new Shader(ShaderType::Unicolor);
	y_shd->setColor("#00FF00");
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(y_shd)));
	std::unique_ptr<SceneObject> y = std::make_unique<SceneObject>(y_model, y_shd, SceneObjectType::Gizmo);
	y->debugName = "v";

	Model* z_model = new Model("resources/models/gizmo/z.obj");
	m_models.push_back(std::unique_ptr<Model>(std::move(z_model)));
	Shader* z_shd = new Shader(ShaderType::Unicolor);
	z_shd->setColor("#0000FF");
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(z_shd)));
	std::unique_ptr<SceneObject> z = std::make_unique<SceneObject>(z_model, z_shd, SceneObjectType::Gizmo);
	z->debugName = "b";

	m_gizmo = std::make_unique<Gizmo>(std::move(x), std::move(y), std::move(z));

	//Fin gizmo

	Shader* red = new Shader(ShaderType::Unicolor);
	red->setColor("#CC0000");
	m_shaders.push_back(std::unique_ptr<Shader>(std::move(red)));
	Selection m_selection = {red, 1.04f};

	SceneObject* backpack = new SceneObject(backpack_mdl, backpack_shd, SceneObjectType::Normal, m_selection);
	backpack->debugName = "backpack";

	SceneObject* ground = new SceneObject(ground_mdl, ground_shd, SceneObjectType::Normal, m_selection);
	ground->debugName = "ground";

	m_selection.scale = 1.15f;
	SceneObject* light = new SceneObject(SceneObjectType::Light, m_selection);
	light->setPointLight(0.09f, 0.032f);
	light->getModel()->translate({0, 1.0f, 2.0f});
	light->debugName = "light";

	m_sceneObjects.push_back(std::unique_ptr<SceneObject>(std::move(backpack)));
	m_sceneObjects.push_back(std::unique_ptr<SceneObject>(std::move(ground)));
	m_sceneObjects.push_back(std::unique_ptr<SceneObject>(std::move(light)));

	HUD* hud = new HUD("arial");
	m_huds.push_back(std::unique_ptr<HUD>(hud));
	hud->setText(std::bind(&Camera::getPositionStr, m_camera.get()));

	HUD* hud2 = new HUD("arial");
	m_huds.push_back(std::unique_ptr<HUD>(hud2));
	hud2->setText(std::bind(&scene::getSelectedObjectCoordinateStr, this));

	m_lights.push_back(*m_sceneObjects[2]);
}

void scene::renderLoop(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime)
{
	static unsigned int i = 0;
	static std::string smoothDT = "";

	g_opengl.glEnable(GL_DEPTH_TEST);
	g_opengl.glEnable(GL_LINE_SMOOTH);
	g_opengl.glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	g_opengl.glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//m_sceneObjects[1]->getModel()->translate({ 0.01, 0, 0 });

	m_camera->move(inputsBeingPressed, deltaTime);

	picking();

	for (auto& sceneObject : m_sceneObjects)
	{
		if (m_selectedObject != sceneObject.get())
		{
			sceneObject->render(*m_camera, getLights());
		}
	}

	if (m_selectedObject != nullptr)
	{
		if (m_selectedObject->debugName != "r" && m_selectedObject->debugName != "b" &&
			m_selectedObject->debugName != "v")
		{
			m_gizmo->setPosition(m_selectedObject->getPosition());
		}
		m_selectedObject->render(*m_camera, getLights());
		g_opengl.glDisable(GL_DEPTH_TEST);
		m_gizmo->render(*m_camera, getLights());
		g_opengl.glEnable(GL_DEPTH_TEST);
	}

	int j = 0;
	for (auto& hud : m_huds)
	{
		if (!(i % 4))
		{
			smoothDT = std::to_string(int(1000 / deltaTime));
		}
		hud->RenderText(1, j * -20, {255, 255, 255}, 0.3);
		j++;
	}
	i++;
}

void scene::walkCamera(Mouvement mouvement, bool active)
{
	switch (mouvement)
	{
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

std::vector<std::unique_ptr<Shader>>& scene::getShaders()
{
	return m_shaders;
}

std::unique_ptr<Camera>& scene::getCamera()
{
	return m_camera;
}

std::vector<std::unique_ptr<Model>>& scene::getModels()
{
	return m_models;
}

std::vector<std::reference_wrapper<SceneObject>> scene::getLights()
{
	return m_lights;
}

void scene::tryToSelect(glm::ivec2 mouseCoords, int viewportWidth, int viewportHeight)
{
	if (!m_pickingTex)
	{
		m_pickingTex = std::make_unique<PickingTexture>(viewportWidth, viewportHeight);
	}
	m_isPicking = true;
	m_mouseCoords = mouseCoords;
}

void scene::picking()
{
	bool hasSelected = false;
	bool gizmoSelected = false;
	if (m_isPicking)
	{
		m_pickingTex->enableWriting();
		if (m_selectedObject)
		{
			g_opengl.glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_gizmo->renderPicking(*m_camera);
			auto id = m_pickingTex->readPixel(m_mouseCoords.x, m_mouseCoords.y);
			if (m_gizmo->isId(id))
			{
				gizmoSelected = true;
			}
		}

		for (auto& sceneObject : m_sceneObjects)
		{
			g_opengl.glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			sceneObject->renderPicking(*m_camera);
			auto id = m_pickingTex->readPixel(m_mouseCoords.x, m_mouseCoords.y);
			if (sceneObject->isId(id))
			{
				if (m_selectedObject != nullptr)
				{
					m_selectedObject->unselect();
				}
				m_selectedObject = sceneObject.get();
				sceneObject->select();
				hasSelected = true;
				gizmoSelected = false;
			}
		}

		m_pickingTex->disableWriting();
	}

	if (!gizmoSelected && m_isPicking && !hasSelected && m_selectedObject != nullptr)
	{
		m_selectedObject->unselect();
		m_selectedObject = nullptr;
	}
	m_isPicking = false;
}

void scene::tryMoveObject(const glm::ivec2& mouseDiff)
{
	if (m_gizmo->isSelected())
	{
		static const float translatingUpFactor = 0.002f;
		if (m_gizmo->getSelectedIndex() == 1)
		{  // up and down
			auto translation = translatingUpFactor * mouseDiff.y *
							   glm::length(m_selectedObject->getPosition() - m_camera->getPosition());
			m_selectedObject->getModel()->translate({0, translation, 0});
		}
		else if (m_gizmo->getSelectedIndex() == 0)
		{
			static const float translatingXFactor = -0.002f;
			const glm::vec3 A{0., 0., 1.};
			const auto B = m_camera->getDirection();
			const auto cosTheta = (glm::dot(A, B)) / (glm::length(A) * glm::length(B));
			const auto translation = translatingXFactor * cosTheta * mouseDiff.x *
									 glm::length(m_selectedObject->getPosition() - m_camera->getPosition());
			m_selectedObject->getModel()->translate({translation, 0, 0});
		}
		else
		{
			static const float translatingYFactor = 0.002f;
			const glm::vec3 A{1., 0., 0.};
			const auto B = m_camera->getDirection();
			const auto cosTheta = (glm::dot(A, B)) / (glm::length(A) * glm::length(B));
			const auto translation = translatingYFactor * cosTheta * mouseDiff.x *
									 glm::length(m_selectedObject->getPosition() - m_camera->getPosition());
			m_selectedObject->getModel()->translate({0, 0, translation});
		}
	}
}

void scene::focusCameraOnSelectedObject(void)
{
	if (m_selectedObject != nullptr)
	{
		const auto selectedObjectPosition = m_selectedObject->getPosition();
		auto newCamPos = selectedObjectPosition - m_camera->getTarget() + m_camera->getPosition();
		newCamPos = glm::normalize(newCamPos - selectedObjectPosition) * 5.0f;
		m_camera->setPosition(selectedObjectPosition + newCamPos);
		m_camera->setTarget(selectedObjectPosition);
	}
}

std::string scene::getSelectedObjectCoordinateStr(void) const
{
	if (m_selectedObject == nullptr)
	{
		return "";
	}
	return "Selected object: " + std::to_string(m_selectedObject->getPosition().x) + "x " +
		   std::to_string(m_selectedObject->getPosition().y) + "y " +
		   std::to_string(m_selectedObject->getPosition().z) + "z ";
	return "";
}