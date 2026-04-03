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
	scene* getscene(void) const;

   private slots:
	void newModelAdded(InfoObject info);

   private:
	std::unique_ptr<App3DViewer> m_app3DViewer;
	std::unique_ptr<scene> m_scene;

	QStandardItemModel* m_sceneObjectModel;
};
