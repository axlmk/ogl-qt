#include <App3D.hpp>
#include <QCoreApplication>
#include <QStandardItemModel>
#include <filesystem>

#include "LightObject.hpp"
#include "SceneObject.hpp"

App3D::App3D(int argc, char* argv[]) : m_app3DViewer{argc, argv, &m_scene}, m_sceneObjectModel{new QStandardItemModel()}
{
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
	auto model = std::make_unique<Model>(appDir.filePath("resources/models/sphere/sphere.obj").toStdU16String());
	model->load();
	m_models.emplace_back(std::move(model));

	auto shader = std::make_unique<Shader>(ShaderType::Light);
	shader->setLight();
	shader->compile();
	m_shaders.emplace_back(std::move(shader));

	const uint modelIdx = static_cast<uint>(m_models.size() - 1);
	const uint shaderIdx = static_cast<uint>(m_shaders.size() - 1);
	const importedObject import{"Point light", modelIdx, shaderIdx, importedObject::Type::Light, LightProperties::LightType::Point};
	m_availableObjects.emplace_back(import);
	m_sceneObjectModel->appendRow(new QStandardItem("Point light"));
}

void App3D::_importModel(InfoObject info)
{
	auto model = std::make_unique<Model>(info.objectPath.toStdString());
	model->load();
	m_models.emplace_back(std::move(model));

	auto shader = std::make_unique<Shader>(ShaderType::Texture);
	shader->addTexture(info.diffusePath.toStdString());
	shader->addTexture(info.specularPath.toStdString());
	shader->compile();
	m_shaders.emplace_back(std::move(shader));

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
			m_scene.addObjectToRenderables(selectedModel.get(), selectedShader.get());
			break;
		case importedObject::Light: {
			m_scene.addLightToRenderables(selectedModel.get(), selectedShader.get(), m_availableObjects[rowIndex].light);
			break;
		}
		default:
			break;
	}
}