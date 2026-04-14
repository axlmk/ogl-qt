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
	void newModelAdded(InfoObject info);

	/**
	 * @brief Emits that a new model should be loaded
	 */
	void modelLoaded(uint modelIndex);

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