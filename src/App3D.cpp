#include <App3D.hpp>



App3D::App3D(int argc, char *argv[]) :
	m_sceneManager{ nullptr },
	m_app3DViewer{ nullptr } {

	m_sceneManager = std::make_unique<SceneManager>();
	m_app3DViewer = std::make_unique<App3DViewer>(argc, argv, m_sceneManager.get());
}



int App3D::run() {
	return m_app3DViewer->run();
}



SceneManager* App3D::getSceneManager() const {
	return m_sceneManager.get();
}