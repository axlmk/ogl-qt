#include <App3DViewer.hpp>
#include <QLabel>


App3DViewer::App3DViewer(int argc, char* argv[], scene* scene)
{
	// Minimal required components

	m_app = std::make_unique<QApplication>(argc, argv);
	m_mainWindow = std::make_unique<QDialog>(nullptr, Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	m_sceneViewer = std::make_unique<SceneViewer>(scene);

	
	// Format management

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);
	m_sceneViewer->setFormat(format);

	// Cursor management
	
	auto globalCenter = m_mainWindow->mapToGlobal(m_sceneViewer->geometry().center());

	// Container widget
	
	QWidget* container = QWidget::createWindowContainer(&(*m_sceneViewer));
	container->setMinimumSize(QSize(800, 600));

	// Layout and widget container
	
	QVBoxLayout* vlay = new QVBoxLayout;
	QWidget* tips = new QWidget();
	QLabel *title = new QLabel("Controls");
	QLabel* controls = new QLabel("Us ethis for that and this for that");
	QVBoxLayout* vlayTips = new QVBoxLayout;
	tips->setLayout(vlayTips);
	vlayTips->setSpacing(5);
	vlayTips->addWidget(title);
	vlayTips->addWidget(controls);

	vlay->setSpacing(0);
	vlay->addWidget(container);
	vlay->addWidget(tips);

	m_mainWindow->setLayout(vlay);
	m_mainWindow->show();
}



int App3DViewer::run() {
	return m_mainWindow->exec();
}