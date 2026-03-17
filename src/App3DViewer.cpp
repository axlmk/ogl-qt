#include <App3DViewer.hpp>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>
#include <QWidget>
#include <Scene.hpp>

App3DViewer::App3DViewer(int argc, char* argv[], scene* scene)
{
	// Minimal required components

	m_app = std::make_unique<QApplication>(argc, argv);
	m_mainWindow = std::make_unique<QDialog>(
		nullptr, Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	m_sceneViewer = std::make_unique<SceneViewer>(scene);

	// Format management

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);
	m_sceneViewer->setFormat(format);

	// Container widget

	QWidget* container = QWidget::createWindowContainer(&(*m_sceneViewer));
	container->setMinimumSize(QSize(1200, 800));

	// Layout and widget container

	std::unique_ptr<QHBoxLayout> globalLayout = std::make_unique<QHBoxLayout>();
	std::unique_ptr<QVBoxLayout> catalogLayout = std::make_unique<QVBoxLayout>();
	QLabel* titreCatalogue = new QLabel("List of imported models");
	QPushButton* importModels = new QPushButton("Import model");
	catalogLayout->addWidget(titreCatalogue);
	catalogLayout->addWidget(importModels);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	QWidget* tips = new QWidget();
	QLabel* title = new QLabel("Controls");
	QLabel* controls = new QLabel("Use this for that and this for that");
	QVBoxLayout* vlayTips = new QVBoxLayout;
	tips->setLayout(vlayTips);
	vlayTips->setSpacing(5);
	vlayTips->addWidget(title);
	vlayTips->addWidget(controls);

	mainLayout->setSpacing(0);
	mainLayout->addWidget(container);
	mainLayout->addWidget(tips);

	globalLayout->addLayout(mainLayout);
	globalLayout->addLayout(catalogLayout.get());

	m_mainWindow->setLayout(globalLayout.get());

	QScreen* screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->availableGeometry();

	QSize windowSize = m_mainWindow->sizeHint();
	int x = screenGeometry.x() + (screenGeometry.width() - windowSize.width()) / 2;
	int y = screenGeometry.y() + (screenGeometry.height() - windowSize.height()) / 2;
	m_mainWindow->move(x, y);
	m_mainWindow->show();
}

int App3DViewer::run(void)
{
	return m_mainWindow->exec();
}