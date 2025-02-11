#include <App3DViewer.hpp>



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
	
	m_sceneViewer->setCursor(QCursor(Qt::BlankCursor));
	auto globalCenter = m_mainWindow->mapToGlobal(m_sceneViewer->geometry().center());
	m_mainWindow->cursor().setPos(globalCenter.x(), globalCenter.y());

	// Container widget
	
	QWidget* container = QWidget::createWindowContainer(&(*m_sceneViewer));
	container->setMinimumSize(QSize(600, 400));

	// Layout and widget container
	
	QVBoxLayout* vlay = new QVBoxLayout;
	vlay->setSpacing(0);
	vlay->addWidget(container);

	m_mainWindow->setLayout(vlay);
	m_mainWindow->show();
}



int App3DViewer::run() {
	return m_mainWindow->exec();
}