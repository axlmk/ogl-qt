#pragma once

#include <QObject>
#include <memory>

#include "App3DViewer.hpp"
#include "Scene.hpp"

class QStandardItemModel;

class App3D : public QObject
{
	Q_OBJECT
   public:
	App3D(int argc, char* argv[]);
	int run(void);

   private slots:

	/**
	 * @brief Import the model to the scene
	 * @param[in] info Information of the object to import
	 */
	void _importModel(InfoObject info);

	/**
	 * @brief Load a model at the \p rowIndex index
	 * @param[in] rowIndex The index of the model to load
	 */
	void _loadModel(uint rowIndex);

	/**
	* @brief Add all the elementary objects to the list of available objects
    */
	void _initializeBasicsObjects(void);

   private:
	/**
	 * @brief Represents an imported object into the project
	 */
	struct importedObject
	{
		std::string name;				   ///< Name
		uint modelIdx;					   ///< 3D model
		uint shaderIdx;					   /// Shader
		enum Type { Light, Normal } type;  ///< Type
		LightProperties::LightType light;  ///< Light properties if it's a light
	};

	scene m_scene;
	App3DViewer m_app3DViewer;

	std::vector<std::unique_ptr<Model>> m_models;
	std::vector<std::unique_ptr<Shader>> m_shaders;
	std::vector<importedObject> m_availableObjects;

	QStandardItemModel* m_sceneObjectModel;
};
