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

#include "ui_App3DViewer.h"

App3DViewer::App3DViewer(Scene* scene) : ui(new Ui::App3DViewer)
{
	ui->setupUi(this);

	// OGL format & configuration
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);

	m_sceneViewer = new SceneViewer(scene);
	m_sceneViewer->setFormat(format);
	m_sceneViewer->setMinimumSize(QSize(900, 600));

	// Replace widget properly in layout
	QLayout* layout = ui->openGLWidget->parentWidget()->layout();
	layout->replaceWidget(ui->openGLWidget, m_sceneViewer);
	ui->openGLWidget->deleteLater();
	ui->openGLWidget = m_sceneViewer;

	// Connections
	connect(ui->pushButton, &QPushButton::clicked, this, &App3DViewer::_openExplorer, Qt::UniqueConnection);
	connect(ui->pushButton_2, &QPushButton::clicked, this, &App3DViewer::_modelSelectedToLoad, Qt::UniqueConnection);
	connect(m_sceneViewer, &SceneViewer::initialized, this, &App3DViewer::initialized, Qt::UniqueConnection);
}

App3DViewer::~App3DViewer()
{
	delete ui;
}

void App3DViewer::centerScreen(void)
{
	QScreen* screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->geometry();
	move((screenGeometry.width() - width()) / 2, 5);
}

void App3DViewer::setSceneObjectsModel(QStandardItemModel* sceneObjectModel)
{
	if (sceneObjectModel != nullptr)
	{
		ui->listView->setModel(sceneObjectModel);
	}
}

void App3DViewer::_openExplorer([[maybe_unused]] bool checked)
{
	auto filePath = QFileDialog::getOpenFileName(nullptr, "Select 3D file", "resources/models", "3D Files (*.obj);;Images (*.jpg)", {},
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
	auto currentIndex = ui->listView->selectionModel()->currentIndex();
	if (!currentIndex.isValid())
	{
		return;
	}
	emit modelLoaded(currentIndex.row());
}

void App3DViewer::initOGLContext(void)
{
	m_sceneViewer->makeCurrent();
}