#pragma once

#include <memory>

#include <QApplication>
#include <QDialog>
#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <SceneViewer.hpp>
#include <SceneManager.hpp>

class App3DViewer {
public:
	App3DViewer(int argc, char *argv[], SceneManager* sceneManager);
	int run();

private:
	std::unique_ptr<QApplication>	m_app;
	std::unique_ptr<QDialog>		m_mainWindow;
	std::unique_ptr<SceneViewer>	m_sceneViewer;
};