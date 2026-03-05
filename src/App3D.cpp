#include <App3D.hpp>

App3D::App3D(int argc, char* argv[]) : m_scene{nullptr}, m_app3DViewer{nullptr} {
	m_scene = std::make_unique<scene>();
	m_app3DViewer = std::make_unique<App3DViewer>(argc, argv, m_scene.get());
}

int App3D::run(void) {
	return m_app3DViewer->run();
}

scene* App3D::getscene(void) const {
	return m_scene.get();
}