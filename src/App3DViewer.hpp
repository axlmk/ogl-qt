#pragma once

#include <QApplication>
#include <QDialog>
#include <SceneViewer.hpp>
#include <memory>

class scene;
class QListView;
class QStandardItemModel;

/**
 * @brief Represents an object that is imported into the project and that can be loaded into the scene
 */
struct InfoObject
{
	QString name;		   ///< Display name of the object
	QString objectPath;	   ///< Path to the object's .obj file
	QString diffusePath;   ///< Path to the object's diffuse file
	QString specularPath;  ///< Path to the object's specular file
};

/**
 * @class The view of the 3DApp, displaying the project's information
 */
class App3DViewer : public QObject
{
	Q_OBJECT
   public:
	/**
	 * @brief Default constructor
	 * @param[in] argc Same thing as the argc from main
	 * @param[in] argv Same thing as the argv from main
	 * @param[in] scene 3D scene of the project
	 */
	App3DViewer(int argc, char* argv[], Scene* scene);

	/**
	 * @brief Starts the loop resposible for executing the windowing system
	 * @return The error code of the Qt window
	 */
	int run(void);

	/**
	 * @brief Set the model of the QListView displaying the object imported into the scene
	 * @param[in] sceneObjectModel The model
	 */
	void setSceneObjectsModel(QStandardItemModel* sceneObjectModel);

   signals:
	/**
	 * @brief Emits that a new model should be added to the project
	 * @param[in] info The information about the added object
	 */
	void newModelAdded(InfoObject info);

	/**
	 * @brief Emits that a new model should be loaded
	 * @param[in] modelIndex The index of the model to load
	 */
	void modelLoaded(uint modelIndex);

	/**
	 * @brief Indicates OpenGL has been initialized
	 */
	void initialized(void);

   private slots:

	/**
	 * @brief Opens up the explorer to imported new object onto the scene
	 * @param[in] checked The state of the button trigger the functino
     */
	void _openExplorer(bool checked);

	/**
	 * @brief Get what modelIndex should be loaded
	 * @param[in] checked The state of the button
	 */
	void _modelSelectedToLoad(bool checked);

   private:
	QApplication m_app;							 ///< The QApplication
	std::unique_ptr<QDialog> m_mainWindow;		 ///< The main Qt dialog of the project
	std::unique_ptr<SceneViewer> m_sceneViewer;	 ///< The Qt OpenGL scene
	QListView* m_sceneObjectView;				 ///< The list view representing the available objects in the project
};