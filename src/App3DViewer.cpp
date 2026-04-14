#include <App3DViewer.hpp>
#include <QFileDialog>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QScreen>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QWidget>
#include <Scene.hpp>

App3DViewer::App3DViewer(int argc, char* argv[], scene* scene)
{
	// Minimal required components

	m_app = std::make_unique<QApplication>(argc, argv);
	m_mainWindow = std::make_unique<QDialog>(nullptr, Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	m_sceneViewer = std::make_unique<SceneViewer>(scene);

	// Format management

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);
	m_sceneViewer->setFormat(format);

	// Container widget

	QWidget* container = QWidget::createWindowContainer(&(*m_sceneViewer));
	container->setMinimumSize(QSize(900, 600));

	// Layout and widget container

	std::unique_ptr<QHBoxLayout> globalLayout = std::make_unique<QHBoxLayout>();
	std::unique_ptr<QVBoxLayout> catalogLayout = std::make_unique<QVBoxLayout>();
	QLabel* titreCatalogue = new QLabel("List of imported models");
	QPushButton* importModels = new QPushButton("Import model");
	connect(importModels, &QPushButton::clicked, this, &App3DViewer::_openExplorer, Qt::UniqueConnection);

	QPushButton* loadModels = new QPushButton("Load model in Scene");
	connect(loadModels, &QPushButton::clicked, this, &App3DViewer::_modelSelectedToLoad, Qt::UniqueConnection);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	QWidget* tips = new QWidget();
	QLabel* title = new QLabel("Controls");
	QLabel* controls = new QLabel(
		"alt + left click : rotate around\nalt + middle click : pan around\nalt + right click : zoom\nClick on object "
		": select it\nClick on gizmo : translate selected object");
	QVBoxLayout* vlayTips = new QVBoxLayout;
	m_sceneObjectView = new QListView();

	catalogLayout->addWidget(titreCatalogue);
	catalogLayout->addWidget(importModels);
	catalogLayout->addWidget(loadModels);
	tips->setLayout(vlayTips);
	vlayTips->setSpacing(5);
	vlayTips->addWidget(title);
	vlayTips->addWidget(controls);
	catalogLayout->addWidget(m_sceneObjectView);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(container);
	mainLayout->addWidget(tips);
	globalLayout->addLayout(mainLayout);
	globalLayout->addLayout(catalogLayout.get());
	m_mainWindow->setLayout(globalLayout.get());

	QScreen* screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->availableGeometry();

	// Set up the application right at the center of the main screen
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

void App3DViewer::setSceneObjectsModel(QStandardItemModel* sceneObjectModel)
{
	if (sceneObjectModel != nullptr)
	{
		m_sceneObjectView->setModel(sceneObjectModel);
	}
}

void App3DViewer::_openExplorer([[maybe_unused]] bool checked)
{
	auto filePath = QFileDialog::getOpenFileName(nullptr, "Select 3D file", {}, "3D Files (*.obj);;Images (*.jpg)", {},
												 QFileDialog::ReadOnly | QFileDialog::DontResolveSymlinks);
	if (filePath.isNull())
	{
		return;
	}
	QFileInfo fileInfo(filePath);
	QDir dir(fileInfo.absolutePath());

	InfoObject info;

	for (const auto& file : dir.entryInfoList(QDir::Files))
	{
		auto name = file.fileName();
		auto extension = file.suffix();
		if (extension == "obj")
		{
			info.objectPath = file.filePath();
			info.name = file.baseName();
		}
		else if (name == "diffuse.jpg" || name == "diffuse.png")
		{
			info.diffusePath = file.filePath();
		}
		else if (name == "specular.jpg" || name == "specular.png")
		{
			info.specularPath = file.filePath();
		}
		else
		{
			QMessageBox msg(nullptr);
			msg.setWindowTitle("Folder doesn't contain compatible 3D project");
			msg.setText(
				"The folder doesn't contain a compatible 3D project. Such a project is composed of:\n"
				"1. An object file with a '.obj' extension\n"
				"2. A diffuse texture called 'diffuse.jpg'\n"
				"3. A specular texture called 'specular.jpg'\n" +
				name);
			msg.exec();
			return;
		}
	}

	emit newModelAdded(info);
}

void App3DViewer::_modelSelectedToLoad([[maybe_unused]] bool checked)
{
	auto currentIndex = m_sceneObjectView->selectionModel()->currentIndex();
	if (!currentIndex.isValid())
	{
		return;
	}
	emit modelLoaded(currentIndex.row());
}