#include <App3D.hpp>
#include <QStandardItemModel>
#include <filesystem>

#include "LightObject.hpp"
#include "SceneObject.hpp"

App3D::App3D(int argc, char* argv[]) : m_app3DViewer{argc, argv, &m_scene}, m_sceneObjectModel{new QStandardItemModel()}
{
	// todo temporary fix for not having to deal with the pointer change when pushing_back
	m_shaders.reserve(20);
	m_models.reserve(20);

	m_app3DViewer.setSceneObjectsModel(m_sceneObjectModel);

	_initializeBasicsObjects();

	connect(&m_app3DViewer, &App3DViewer::newModelAdded, this, &App3D::newModelAdded, Qt::UniqueConnection);
	connect(&m_app3DViewer, &App3DViewer::modelLoaded, this, &App3D::loadModel, Qt::UniqueConnection);
}

int App3D::run(void)
{
	return m_app3DViewer.run();
}

void App3D::newModelAdded(InfoObject info)
{
	m_models.emplace_back(Model(info.objectPath.toStdString()));

	m_shaders.emplace_back(Shader(ShaderType::Texture));
	m_shaders.back().addTexture(info.diffusePath.toStdString());
	m_shaders.back().addTexture(info.specularPath.toStdString());

	// todo test everything went ok
	auto sceneObject = std::make_unique<SceneObject>(&m_models.back(), &m_shaders.back());
	sceneObject->setName(info.name.toStdString());
	m_availableObjects.emplace_back(std::move(sceneObject));

	m_sceneObjectModel->appendRow(new QStandardItem(info.name));
}

void App3D::_initializeBasicsObjects(void)
{
	m_models.emplace_back(Model("resources/models/sphere/sphere.obj"));
	m_shaders.emplace_back(Shader(ShaderType::Light));
	m_shaders.back().setLight();

	auto pointLight = std::make_unique<LightObject>(&m_models.back(), &m_shaders.back());
	pointLight->setPointLight(0.09f, 0.032f);
	pointLight->setName("Point Light");
	m_availableObjects.emplace_back(std::move(pointLight));
	m_sceneObjectModel->appendRow(new QStandardItem("Point Light"));
}

void App3D::loadModel(uint rowIndex)
{
	auto objectToLoad = m_availableObjects[rowIndex].get();
	objectToLoad->load();
	m_scene.addObjectToRenderables(objectToLoad);
}