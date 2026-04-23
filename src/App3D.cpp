#include <App3D.hpp>
#include <QCoreApplication>
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

	appDir = QDir(QCoreApplication::applicationDirPath());

	connect(&m_app3DViewer, &App3DViewer::newModelAdded, this, &App3D::_importModel, Qt::UniqueConnection);
	connect(&m_app3DViewer, &App3DViewer::modelLoaded, this, &App3D::_loadModel, Qt::UniqueConnection);
	connect(&m_app3DViewer, &App3DViewer::initialized, this, &App3D::_initializeBasicsObjects, Qt::UniqueConnection);
}

int App3D::run(void)
{
	return m_app3DViewer.run();
}

void App3D::_initializeBasicsObjects(void)
{

	m_models.emplace_back(Model(appDir.filePath("resources/models/sphere/sphere.obj").toStdU16String()));
	m_models.back().load();

	m_shaders.emplace_back(Shader(ShaderType::Light));
	m_shaders.back().setLight();
	m_shaders.back().compile();

	const uint modelIdx = static_cast<uint>(m_models.size() - 1);
	const uint shaderIdx = static_cast<uint>(m_shaders.size() - 1);
	const importedObject import{"Point light", modelIdx, shaderIdx, importedObject::Type::Light, LightProperties::LightType::Point};
	m_availableObjects.emplace_back(import);
	m_sceneObjectModel->appendRow(new QStandardItem("Point light"));
}

void App3D::_importModel(InfoObject info)
{
	m_models.emplace_back(Model(info.objectPath.toStdString()));
	m_models.back().load();

	m_shaders.emplace_back(Shader(ShaderType::Texture));
	m_shaders.back().addTexture(info.diffusePath.toStdString());
	m_shaders.back().addTexture(info.specularPath.toStdString());
	m_shaders.back().compile();

	// todo test everything went ok

	const uint modelIdx = static_cast<uint>(m_models.size() - 1);
	const uint shaderIdx = static_cast<uint>(m_shaders.size() - 1);
	const importedObject import{info.name.toStdString(), modelIdx, shaderIdx, importedObject::Type::Normal, LightProperties::LightType::Point};
	m_availableObjects.emplace_back(import);
	m_sceneObjectModel->appendRow(new QStandardItem(info.name));
}

void App3D::_loadModel(uint rowIndex)
{
	auto& selectedModel = m_models.at(m_availableObjects[rowIndex].modelIdx);
	auto& selectedShader = m_shaders.at(m_availableObjects[rowIndex].shaderIdx);

	switch (m_availableObjects[rowIndex].type)
	{
		case importedObject::Type::Normal:
			m_scene.addObjectToRenderables(&selectedModel, &selectedShader);
			break;
		case importedObject::Light: {
			m_scene.addLightToRenderables(&selectedModel, &selectedShader, m_availableObjects[rowIndex].light);
			break;
		}
		default:
			break;
	}
}