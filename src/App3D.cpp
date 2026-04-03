#include <App3D.hpp>
#include <QStandardItemModel>

App3D::App3D(int argc, char* argv[]) : m_app3DViewer{nullptr}, m_scene{nullptr}
{
	m_scene = std::make_unique<scene>();
	m_app3DViewer = std::make_unique<App3DViewer>(argc, argv, m_scene.get());
	connect(m_app3DViewer.get(), &App3DViewer::newModelAdded, this, &App3D::newModelAdded, Qt::UniqueConnection);

	m_sceneObjectModel = new QStandardItemModel(0, 2);
	m_sceneObjectModel->appendRow(new QStandardItem("Point Light"));
	m_sceneObjectModel->appendRow(new QStandardItem("Guitar"));

	m_app3DViewer->setSceneObjectsModel(m_sceneObjectModel);
}

int App3D::run(void)
{
	return m_app3DViewer->run();
}

scene* App3D::getscene(void) const
{
	return m_scene.get();
}

void App3D::newModelAdded(InfoObject info)
{
	// construire un scene object non loaded et l'ajouter dans une liste. cette liste donnera a manger au m_sceneobjectModel par la suite
}