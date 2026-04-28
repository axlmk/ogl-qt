#pragma once

#include <QObject>
#include <memory>

#include "App3DViewer.hpp"
#include "Scene.hpp"

class QStandardItemModel;

/**
 * @class Handle the interaction between the interface and the OpenGL scene
 */
class App3D : public QObject
{
	Q_OBJECT
   public:
	/**
	 * @brief Default constructor
	 * @param[in] argc Same thing as the argc from main
	 * @param[in] argv Same thing as the argv from main
	 */
	App3D(int argc, char* argv[]);

	/**
	 * @brief Starts the loop resposible for executing the windowing system
	 * @return The error code of the Qt window
	 */
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

	Scene m_scene;				///< The scene containing the logic of the elements to render
	App3DViewer m_app3DViewer;	///< The window responsible for rendering the scene

	std::vector<std::unique_ptr<Model>> m_models;	 ///< Contains all the models of the project
	std::vector<std::unique_ptr<Shader>> m_shaders;	 ///< Contains all the shaders of the project
	std::vector<importedObject> m_availableObjects;	 ///< Contains all the objects that can be added to the scene

	QStandardItemModel* m_sceneObjectModel;	 ///< The Qt model holding references to the objects of the project
};
