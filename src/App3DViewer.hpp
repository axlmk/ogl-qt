#pragma once

#include <QApplication>
#include <QDialog>
#include <SceneViewer.hpp>
#include <memory>

class scene;

class App3DViewer {
   public:
	App3DViewer(int argc, char* argv[], scene* scene);
	int run(void);

   private:
	std::unique_ptr<QApplication> m_app;
	std::unique_ptr<QDialog> m_mainWindow;
	std::unique_ptr<SceneViewer> m_sceneViewer;
};