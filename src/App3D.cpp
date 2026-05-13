#include <App3D.hpp>
#include <QCoreApplication>
#include <QStandardItemModel>
#include <QWindow>
#include <filesystem>

#include "LightObject.hpp"
#include "SceneObject.hpp"

App3D::App3D(int argc, char* argv[])
{
	m_app = new QApplication(argc, argv);
	m_app3DViewer = new App3DViewer(&m_scene);
	m_sceneObjectModel = new QStandardItemModel();
	appDir = QDir(QCoreApplication::applicationDirPath());

	connect(m_app3DViewer, &App3DViewer::newModelAdded, this, &App3D::_importModel, Qt::UniqueConnection);
	connect(m_app3DViewer, &App3DViewer::modelLoaded, this, &App3D::_loadModel, Qt::UniqueConnection);
	connect(m_app3DViewer, &App3DViewer::initialized, this, &App3D::_initializeBasicsObjects);

	m_app3DViewer->show();
	m_app3DViewer->setSceneObjectsModel(m_sceneObjectModel);

	// Icon
	m_app->setWindowIcon(QIcon(appDir.filePath("resources/icons/app_icon.png")));
}

int App3D::run(void)
{
	return m_app->exec();
}

void App3D::_initializeBasicsObjects(void)
{
	_importLight("Point light", LightProperties::LightType::Point, "sphere");
	_importLight("Directional light", LightProperties::LightType::Directional, "directional");
	_importLight("Spot light", LightProperties::LightType::Spot, "spot");

	m_app3DViewer->centerScreen();
}

void App3D::_importLight(std::string name, LightProperties::LightType lightType, std::string objectName)
{
	auto model = std::make_unique<Model>(appDir.filePath(QString("resources/models/%1/%1.obj").arg(QString::fromStdString(objectName))).toStdU16String());
	model->load();
	m_models.emplace_back(std::move(model));

	auto shader = std::make_unique<Shader>(ShaderType::Light);
	shader->setLight();
	shader->compile();
	m_shaders.emplace_back(std::move(shader));

	const uint modelIdx = static_cast<uint>(m_models.size() - 1);
	const uint shaderIdx = static_cast<uint>(m_shaders.size() - 1);
	const importedObject import{name, modelIdx, shaderIdx, importedObject::Type::Light, lightType};
	m_availableObjects.emplace_back(import);
	m_sceneObjectModel->appendRow(new QStandardItem(QString::fromStdString(name)));
}

void App3D::_importModel(InfoObject info)
{
	m_app3DViewer->initOGLContext();
	auto model = std::make_unique<Model>(info.objectPath.toStdString());
	model->load();
	m_models.emplace_back(std::move(model));

	auto shader = std::make_unique<Shader>(ShaderType::Texture);
	shader->addTexture(info.diffusePath.toStdString());
	shader->addTexture(info.specularPath.toStdString());
	shader->compile();
	m_shaders.emplace_back(std::move(shader));

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
			m_scene.addObjectToRenderables(selectedModel.get(), selectedShader.get(), m_availableObjects[rowIndex].name);
			break;
		case importedObject::Light: {
			m_scene.addLightToRenderables(selectedModel.get(), selectedShader.get(), m_availableObjects[rowIndex].light);
			break;
		}
		default:
			break;
	}
}