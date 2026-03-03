#pragma once

#include <memory>

#include <App3DViewer.hpp>
// #include <Scene.hpp>

class App3D
{
public:
	App3D(int argc, char *argv[]);
	int run();
	// scene* getscene() const;

private:
	std::unique_ptr<App3DViewer> m_app3DViewer;
	// std::unique_ptr<scene> m_scene;
};
