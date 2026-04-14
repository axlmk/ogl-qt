#pragma once

#include <QObject>
#include <memory>

#include "App3DViewer.hpp"
#include "Scene.hpp"

class QStandardItemModel;
class SceneObject;

class App3D : public QObject
{
	Q_OBJECT
   public:
	App3D(int argc, char* argv[]);
	int run(void);

   private slots:
	void newModelAdded(InfoObject info);

	/**
	 * @brief Load a model at the \p rowIndex index
	 * @param[in] rowIndex The index of the model to load
	 */
	void loadModel(uint rowIndex);

   private:
	/**
	* @brief Add all the elementary objects to the list of available objects
    */
	void _initializeBasicsObjects(void);

	scene m_scene;
	App3DViewer m_app3DViewer;

	std::vector<Model> m_models;
	std::vector<Shader> m_shaders;
	std::vector<std::unique_ptr<SceneObject>> m_availableObjects;

	QStandardItemModel* m_sceneObjectModel;
};
