#include <App3DViewer.hpp>
#include <QFileDialog>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
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

#define FLOAT_PRECISION 0.01f

App3DViewer::App3DViewer(Scene* scene) : m_selectedObject{nullptr}, ui(new Ui::App3DViewer)
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
	connect(m_sceneViewer, &SceneViewer::objectSelectedChanged, this, &App3DViewer::_selectedObjectChanged, Qt::UniqueConnection);
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

void App3DViewer::_selectedObjectChanged(SceneObject* selectedObject)
{
	if (selectedObject == nullptr)
	{
		for (int i = ui->gridLayout->count() - 1; i >= 0; i--)
		{
			auto item = ui->gridLayout->itemAt(i);
			item->widget()->deleteLater();
		}
		m_selectedObject = nullptr;
		return;
	}

	if (m_selectedObject != selectedObject)
	{
		for (int i = ui->gridLayout->count() - 1; i >= 0; i--)
		{
			auto item = ui->gridLayout->itemAt(i);
			item->widget()->deleteLater();
		}

		QLabel* nameLabel = new QLabel(QString::fromStdString(selectedObject->getName()));
		ui->gridLayout->addWidget(nameLabel, 0, 0);

		QLabel* positionLabel = new QLabel("Position:");
		ui->gridLayout->addWidget(positionLabel, 1, 0);

		auto position = selectedObject->getPosition();

		QLabel* positionLabelX = new QLabel("x:");
		ui->gridLayout->addWidget(positionLabelX, 2, 0);
		QLineEdit* positionXEdit = new QLineEdit(QString::number(position.x, 'g', 3));
		connect(positionXEdit, &QLineEdit::textChanged, this, [&position](const QString& text) { position.x = text.toFloat(); });
		ui->gridLayout->addWidget(positionXEdit, 2, 1);

		QLabel* positionLabelY = new QLabel("y:");
		ui->gridLayout->addWidget(positionLabelY, 3, 0);
		QLineEdit* positionYEdit = new QLineEdit(QString::number(position.y, 'g', 3));
		connect(positionYEdit, &QLineEdit::textChanged, this, [&position](const QString& text) { position.y = text.toFloat(); });
		ui->gridLayout->addWidget(positionYEdit, 3, 1);

		QLabel* positionLabelZ = new QLabel("z:");
		ui->gridLayout->addWidget(positionLabelZ, 4, 0);
		QLineEdit* positionZEdit = new QLineEdit(QString::number(position.z, 'g', 3));
		connect(positionZEdit, &QLineEdit::textChanged, this, [&position](const QString& text) { position.z = text.toFloat(); });
		ui->gridLayout->addWidget(positionZEdit, 4, 1);

		auto lightObj = dynamic_cast<LightObject*>(selectedObject);
		if (lightObj != nullptr)
		{
			auto light = lightObj->getLightProperties();
			switch (light->type)
			{
				case LightProperties::LightType::Spot: {
					QLabel* cutoffLabel = new QLabel("Cutoff:");
					ui->gridLayout->addWidget(cutoffLabel, 6, 0);
					QLineEdit* cutoffEdit = new QLineEdit(QString::number(light->cutoff, 'g', 3));
					ui->gridLayout->addWidget(cutoffEdit, 6, 1);

					QLabel* outerCutoffLabel = new QLabel("Outer cutoff:");
					ui->gridLayout->addWidget(outerCutoffLabel, 7, 0);
					QLineEdit* outerCutoffEdit = new QLineEdit(QString::number(light->outerCutoff, 'g', 3));
					ui->gridLayout->addWidget(outerCutoffEdit, 7, 1);
				}
				break;
				case LightProperties::LightType::Point: {
					QLabel* linearLabel = new QLabel("Linear decrease:");
					ui->gridLayout->addWidget(linearLabel, 6, 0);
					QLineEdit* linearEdit = new QLineEdit(QString::number(light->linear, 'g', 3));
					ui->gridLayout->addWidget(linearEdit, 6, 1);
					connect(linearEdit, &QLineEdit::textChanged, this, [light](const QString& text) { light->linear = text.toFloat(); });

					QLabel* quadraticLabel = new QLabel("Quadratic decrease:");
					ui->gridLayout->addWidget(quadraticLabel, 7, 0);
					QLineEdit* quadraticEdit = new QLineEdit(QString::number(light->quadratic, 'g', 3));
					ui->gridLayout->addWidget(quadraticEdit, 7, 1);
					connect(linearEdit, &QLineEdit::textChanged, this, [light](const QString& text) { light->linear = text.toFloat(); });
				}
				break;
				case LightProperties::LightType::Directional: {
					QLabel* directionLabel = new QLabel("Direction:");
					ui->gridLayout->addWidget(directionLabel, 6, 0);

					QLabel* directionXLabel = new QLabel("x:");
					ui->gridLayout->addWidget(directionXLabel, 7, 0);
					QLineEdit* directionXEdit = new QLineEdit(QString::number(light->direction.x, 'g', 3));
					ui->gridLayout->addWidget(directionXEdit, 7, 1);

					QLabel* directionYLabel = new QLabel("y:");
					ui->gridLayout->addWidget(directionYLabel, 8, 0);
					QLineEdit* directionYEdit = new QLineEdit(QString::number(light->direction.y, 'g', 3));
					ui->gridLayout->addWidget(directionYEdit, 8, 1);

					QLabel* directionZLabel = new QLabel("z:");
					ui->gridLayout->addWidget(directionZLabel, 9, 0);
					QLineEdit* directionZEdit = new QLineEdit(QString::number(light->direction.z, 'g', 3));
					ui->gridLayout->addWidget(directionZEdit, 9, 1);
				}
				default:
					break;
			}
		}

		m_selectedObject = selectedObject;
	}
	else
	{
		auto* editX = qobject_cast<QLineEdit*>(ui->gridLayout->itemAtPosition(2, 1)->widget());
		auto xDiff = selectedObject->getPosition().x - editX->text().toFloat();
		if (xDiff >= FLOAT_PRECISION || xDiff <= -FLOAT_PRECISION)
		{
			editX->setText(QString::number(selectedObject->getPosition().x, 'g', 3));
		}

		auto* editY = qobject_cast<QLineEdit*>(ui->gridLayout->itemAtPosition(3, 1)->widget());
		auto yDiff = selectedObject->getPosition().y - editY->text().toFloat();
		if (yDiff >= FLOAT_PRECISION || yDiff <= -FLOAT_PRECISION)
		{
			editY->setText(QString::number(selectedObject->getPosition().y, 'g', 3));
		}

		auto* editZ = qobject_cast<QLineEdit*>(ui->gridLayout->itemAtPosition(4, 1)->widget());
		auto zDiff = selectedObject->getPosition().z - editZ->text().toFloat();
		if (zDiff >= FLOAT_PRECISION || zDiff <= -FLOAT_PRECISION)
		{
			editZ->setText(QString::number(selectedObject->getPosition().z, 'g', 3));
		}
	}
}