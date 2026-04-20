#pragma once

#include <QApplication>
#include <QDialog>
#include <SceneViewer.hpp>
#include <memory>

class scene;
class QListView;
class QStandardItemModel;

struct InfoObject
{
	QString name;
	QString objectPath;
	QString diffusePath;
	QString specularPath;
};

class App3DViewer : public QObject
{
	Q_OBJECT
   public:
	App3DViewer(int argc, char* argv[], scene* scene);
	int run(void);

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

	void _openExplorer(bool checked);

	/**
	 * @brief Get what modelIndex should be loaded
	 * @param[in] checked The state of the button
	 */
	void _modelSelectedToLoad(bool checked);

   private:
	std::unique_ptr<QApplication> m_app;
	std::unique_ptr<QDialog> m_mainWindow;
	std::unique_ptr<SceneViewer> m_sceneViewer;
	QListView* m_sceneObjectView;
};