#include <Scene.hpp>
#include <SceneViewer.hpp>

scene::scene() : m_camera{SpaceCoord(0.0, 0.0, 3.0)}, m_selectedObject{nullptr}, m_cameraDirection{false, false, false, false}, m_numberOfCreatedObjects{0} {}

void scene::initializeScene(int viewportWidth, int viewportHeight)
{
	m_selection = {{ShaderType::Unicolor}, 1.05f};
	m_selection.color.setColor("#CC0000");
	m_selection.color.compile();

	SceneObject::pick.setColor("#ffffff");
	SceneObject::pick.compile();

	m_gizmo.load();

	m_pickingTex = PickingTexture(viewportWidth, viewportHeight);

	auto hud = std::make_unique<HUD>("arial");
	hud->setText(std::bind(&Camera::getPositionStr, &m_camera));
	m_huds.push_back(std::move(hud));

	auto hud2 = std::make_unique<HUD>("arial");
	hud2->setText(std::bind(&scene::getSelectedObjectCoordinateStr, this));
	m_huds.push_back(std::move(hud2));
}

void scene::updateViewport(int width, int height)
{
	m_pickingTex.updateTexture(width, height);
}

void scene::renderLoop(std::unordered_map<std::string, bool> inputsBeingPressed, qint64 deltaTime)
{
	g_opengl.glEnable(GL_DEPTH_TEST);
	g_opengl.glEnable(GL_LINE_SMOOTH);
	g_opengl.glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	g_opengl.glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_camera.move(inputsBeingPressed, deltaTime);

	_picking();	 // Perform the picking rendering to know if an object is selected or not

	for (const auto& sceneObject : m_renderedObjects)
	{
		if (sceneObject.get() != m_selectedObject)
		{
			sceneObject->render(m_camera, m_lights);
		}
	}

	if (m_selectedObject != nullptr)
	{
		m_gizmo.setPosition(m_selectedObject->getPosition());
		m_selectedObject->renderSelected(m_camera, m_lights, m_selection);
		g_opengl.glDisable(GL_DEPTH_TEST);
		m_gizmo.render(m_camera, m_lights);
		g_opengl.glEnable(GL_DEPTH_TEST);
	}

	// HUD management
	static unsigned int i = 0;
	static std::string smoothDT = "";
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

void scene::addObjectToRenderables(Model* model, Shader* shader, const std::string& name)
{
	auto objectToAdd = std::make_unique<SceneObject>(model, shader);
	objectToAdd->setName(name + std::to_string(m_numberOfCreatedObjects));
	m_renderedObjects.push_back(std::move(objectToAdd));
	m_numberOfCreatedObjects++;
}

void scene::addLightToRenderables(Model* model, Shader* shader, LightProperties::LightType light)
{
	if (m_lights.size() >= MAX_LIGHT_NUMBER)
	{
		qWarning() << "No more light can be added to the scene. Max number of lights:" << MAX_LIGHT_NUMBER;
		return;
	}

	auto lightToAdd = std::make_unique<LightObject>(model, shader);
	switch (light)
	{
		case LightProperties::LightType::Point:
			lightToAdd->setPointLight(0.09f, 0.032f);
			lightToAdd->setName("Point Light" + m_numberOfCreatedObjects);
			break;
		case LightProperties::LightType::Directional:
			lightToAdd->setDirectionalLight(glm::vec3(0.f, -1.0f, 0.0f));
			lightToAdd->setName("Directionnal Light" + m_numberOfCreatedObjects);
			break;
		default:
			lightToAdd->setSpotLight(glm::vec3(0.f, -1.0f, 0.0f), 0.91, 0.82);
			lightToAdd->setName("Spot Light" + m_numberOfCreatedObjects);
			break;
	}

	m_lights.push_back(lightToAdd->getLightProperties());
	m_renderedObjects.push_back(std::move(lightToAdd));

	m_numberOfCreatedObjects++;
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

Camera& scene::getCamera(void)
{
	return m_camera;
}

void scene::enablePicking(glm::ivec2 mouseCoords)
{
	m_isPicking = true;
	m_mouseCoords = mouseCoords;
}

void scene::disablePicking(void)
{
	m_gizmo.unselect();
}

void scene::_picking()
{
	bool hasSelected = false;
	bool gizmoSelected = false;

	// Are we clicking in the scene?
	if (m_isPicking)
	{
		m_pickingTex.enableWriting();

		// Are we trying to select the gizmo?
		if (m_selectedObject)
		{
			g_opengl.glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_gizmo.renderPicking(m_camera);
			glm::ivec3 id = m_pickingTex.readPixel(m_mouseCoords.x, m_mouseCoords.y);
			if (m_gizmo.isId(id))
			{
				m_gizmo.select(id);
				gizmoSelected = true;
			}
		}

		// Something else than the gizmo has been selected
		if (!gizmoSelected)
		{
			m_gizmo.unselect();
			g_opengl.glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			g_opengl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (auto it = m_renderedObjects.cbegin(); it != m_renderedObjects.cend(); it++)
			{
				auto sceneObject = (*it).get();
				sceneObject->renderPicking(m_camera);
				glm::ivec3 id = m_pickingTex.readPixel(m_mouseCoords.x, m_mouseCoords.y);
				if (sceneObject->isId(id))
				{
					m_selectedObject = sceneObject;
					hasSelected = true;
				}
			}
		}

		m_pickingTex.disableWriting();

		if ((!gizmoSelected) && (!hasSelected) && (m_selectedObject != nullptr))
		{
			m_selectedObject = nullptr;
		}
	}

	m_isPicking = false;
}

void scene::tryMoveObject(const glm::ivec2& mouseDiff)
{
	if (m_gizmo.isSelected())
	{
		if (m_gizmo.getSelectedIndex() == 1)
		{
			static const float translatingUpFactor = 0.002f;
			auto translation = translatingUpFactor * mouseDiff.y * glm::length(m_selectedObject->getPosition() - m_camera.getPosition());
			m_selectedObject->translate({0, translation, 0});
		}
		else if (m_gizmo.getSelectedIndex() == 0)
		{
			static const float translatingXFactor = -0.002f;
			const glm::vec3 A{0., 0., 1.};
			const auto B = m_camera.getDirection();
			const auto cosTheta = (glm::dot(A, B)) / (glm::length(A) * glm::length(B));
			const auto translation = translatingXFactor * cosTheta * mouseDiff.x * glm::length(m_selectedObject->getPosition() - m_camera.getPosition());
			m_selectedObject->translate({translation, 0, 0});
		}
		else
		{
			static const float translatingYFactor = 0.002f;
			const glm::vec3 A{1., 0., 0.};
			const auto B = m_camera.getDirection();
			const auto cosTheta = (glm::dot(A, B)) / (glm::length(A) * glm::length(B));
			const auto translation = translatingYFactor * cosTheta * mouseDiff.x * glm::length(m_selectedObject->getPosition() - m_camera.getPosition());
			m_selectedObject->translate({0, 0, translation});
		}
	}
}

void scene::focusCameraOnSelectedObject(void)
{
	if (m_selectedObject != nullptr)
	{
		const auto selectedObjectPosition = m_selectedObject->getPosition();
		auto newCamPos = selectedObjectPosition - m_camera.getTarget() + m_camera.getPosition();
		newCamPos = glm::normalize(newCamPos - selectedObjectPosition) * 5.0f;
		m_camera.setPosition(selectedObjectPosition + newCamPos);
		m_camera.setTarget(selectedObjectPosition);
	}
}

std::string scene::getSelectedObjectCoordinateStr(void) const
{
	if (m_selectedObject == nullptr)
	{
		return "";
	}
	return "Selected object: " + std::to_string(m_selectedObject->getPosition().x) + "x " + std::to_string(m_selectedObject->getPosition().y) + "y " +
		   std::to_string(m_selectedObject->getPosition().z) + "z ";
}