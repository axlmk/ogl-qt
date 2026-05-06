#include <App3DViewer.hpp>
#include <QDoubleSpinBox>
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

		QLabel* positionLabelX = new QLabel("x:");
		ui->gridLayout->addWidget(positionLabelX, 2, 0);
		QDoubleSpinBox* positionXEdit = new QDoubleSpinBox();
		positionXEdit->setRange(-1000.0, 1000.0);
		positionXEdit->setSingleStep(0.1);
		positionXEdit->setValue(selectedObject->getPosition().x);
		connect(positionXEdit, &QDoubleSpinBox::valueChanged, this, [selectedObject, positionXEdit](double value) {
			selectedObject->setPosition(glm::vec3(value, selectedObject->getPosition().y, selectedObject->getPosition().z));
		});
		ui->gridLayout->addWidget(positionXEdit, 2, 1);

		QLabel* positionLabelY = new QLabel("y:");
		ui->gridLayout->addWidget(positionLabelY, 3, 0);
		QDoubleSpinBox* positionYEdit = new QDoubleSpinBox();
		positionYEdit->setRange(-1000.0, 1000.0);
		positionYEdit->setSingleStep(0.1);
		positionYEdit->setValue(selectedObject->getPosition().y);
		connect(positionYEdit, &QDoubleSpinBox::valueChanged, this, [selectedObject, positionYEdit](double value) {
			selectedObject->setPosition(glm::vec3(selectedObject->getPosition().x, value, selectedObject->getPosition().z));
		});
		ui->gridLayout->addWidget(positionYEdit, 3, 1);

		QLabel* positionLabelZ = new QLabel("z:");
		ui->gridLayout->addWidget(positionLabelZ, 4, 0);
		QDoubleSpinBox* positionZEdit = new QDoubleSpinBox();
		positionZEdit->setRange(-1000.0, 1000.0);
		positionZEdit->setSingleStep(0.1);
		positionZEdit->setValue(selectedObject->getPosition().z);
		connect(positionZEdit, &QDoubleSpinBox::valueChanged, this, [selectedObject, positionZEdit](double value) {
			selectedObject->setPosition(glm::vec3(selectedObject->getPosition().x, selectedObject->getPosition().y, value));
		});
		ui->gridLayout->addWidget(positionZEdit, 4, 1);

		auto lightObj = dynamic_cast<LightObject*>(selectedObject);
		if (lightObj != nullptr)
		{
			auto light = lightObj->getLightProperties();

			QLabel* intensityLabel = new QLabel("Intensity:");
			ui->gridLayout->addWidget(intensityLabel, 5, 0);
			QDoubleSpinBox* intensityEdit = new QDoubleSpinBox();
			intensityEdit->setRange(0.0, 10.0);
			intensityEdit->setSingleStep(0.1);
			intensityEdit->setValue(light->intensity);
			ui->gridLayout->addWidget(intensityEdit, 5, 1);
			connect(intensityEdit, &QDoubleSpinBox::valueChanged, this, [light, intensityEdit](double value) { light->intensity = value; });
			uint additionalRow = 0;
			switch (light->type)
			{
				case LightProperties::LightType::Point: {
					QLabel* linearLabel = new QLabel("Linear decrease:");
					ui->gridLayout->addWidget(linearLabel, 6, 0);
					QDoubleSpinBox* linearEdit = new QDoubleSpinBox();
					linearEdit->setRange(0.0, 10.0);
					linearEdit->setSingleStep(0.1);
					linearEdit->setValue(light->linear);
					ui->gridLayout->addWidget(linearEdit, 6, 1);
					connect(linearEdit, &QDoubleSpinBox::valueChanged, this, [light, linearEdit](double value) { light->linear = value; });

					QLabel* quadraticLabel = new QLabel("Quadratic decrease:");
					ui->gridLayout->addWidget(quadraticLabel, 7, 0);
					QDoubleSpinBox* quadraticEdit = new QDoubleSpinBox();
					quadraticEdit->setRange(0.0, 10.0);
					quadraticEdit->setSingleStep(0.1);
					quadraticEdit->setValue(light->quadratic);
					ui->gridLayout->addWidget(quadraticEdit, 7, 1);
					connect(quadraticEdit, &QDoubleSpinBox::valueChanged, this, [light, quadraticEdit](double value) { light->quadratic = value; });
					break;
				}
				case LightProperties::LightType::Spot: {
					QLabel* cutoffLabel = new QLabel("Cutoff:");
					ui->gridLayout->addWidget(cutoffLabel, 6, 0);
					QDoubleSpinBox* cutoffEdit = new QDoubleSpinBox();
					cutoffEdit->setRange(0.0, 180.0);
					cutoffEdit->setSingleStep(1);
					cutoffEdit->setValue(light->cutoff);
					ui->gridLayout->addWidget(cutoffEdit, 6, 1);
					connect(cutoffEdit, &QDoubleSpinBox::valueChanged, this, [light, cutoffEdit](double value) { light->cutoff = value; });

					QLabel* outerCutoffLabel = new QLabel("Outer cutoff:");
					ui->gridLayout->addWidget(outerCutoffLabel, 7, 0);
					QDoubleSpinBox* outerCutoffEdit = new QDoubleSpinBox();
					outerCutoffEdit->setRange(0.0, 180.0);
					outerCutoffEdit->setSingleStep(1);
					outerCutoffEdit->setValue(light->outerCutoff);
					ui->gridLayout->addWidget(outerCutoffEdit, 7, 1);
					connect(outerCutoffEdit, &QDoubleSpinBox::valueChanged, this, [light, outerCutoffEdit](double value) { light->outerCutoff = value; });
					additionalRow = 2;
					[[fallthrough]];
				}
				case LightProperties::LightType::Directional: {
					QLabel* directionLabel = new QLabel("Direction:");
					ui->gridLayout->addWidget(directionLabel, 6 + additionalRow, 0);

					QLabel* directionXLabel = new QLabel("x:");
					ui->gridLayout->addWidget(directionXLabel, 7 + additionalRow, 0);
					QDoubleSpinBox* directionXEdit = new QDoubleSpinBox();
					directionXEdit->setRange(-180.0, 180.0);
					directionXEdit->setSingleStep(1);
					directionXEdit->setValue(selectedObject->getRotation().x);
					ui->gridLayout->addWidget(directionXEdit, 7 + additionalRow, 1);
					connect(directionXEdit, &QDoubleSpinBox::valueChanged, this, [selectedObject, directionXEdit, lightObj](double value) {
						lightObj->setLightDirection(glm::vec3(value, selectedObject->getRotation().y, selectedObject->getRotation().z));
					});

					QLabel* directionYLabel = new QLabel("y:");
					ui->gridLayout->addWidget(directionYLabel, 8 + additionalRow, 0);
					QDoubleSpinBox* directionYEdit = new QDoubleSpinBox();
					directionYEdit->setRange(-180.0, 180.0);
					directionYEdit->setSingleStep(1);
					directionYEdit->setValue(selectedObject->getRotation().y);
					ui->gridLayout->addWidget(directionYEdit, 8 + additionalRow, 1);
					connect(directionYEdit, &QDoubleSpinBox::valueChanged, this, [selectedObject, directionYEdit, lightObj](double value) {
						lightObj->setLightDirection(glm::vec3(selectedObject->getRotation().x, value, selectedObject->getRotation().z));
					});

					QLabel* directionZLabel = new QLabel("z:");
					ui->gridLayout->addWidget(directionZLabel, 9 + additionalRow, 0);
					QDoubleSpinBox* directionZEdit = new QDoubleSpinBox();
					directionZEdit->setRange(-180.0, 180.0);
					directionZEdit->setSingleStep(1);
					directionZEdit->setValue(selectedObject->getRotation().z);
					ui->gridLayout->addWidget(directionZEdit, 9 + additionalRow, 1);
					connect(directionZEdit, &QDoubleSpinBox::valueChanged, this, [selectedObject, directionZEdit, lightObj](double value) {
						lightObj->setLightDirection(glm::vec3(selectedObject->getRotation().x, selectedObject->getRotation().y, value));
					});
					break;
				}
				default:
					break;
			}
		}

		m_selectedObject = selectedObject;
	}
	else
	{
		// Required to update the spinboxes if the position of the object is changed by moving it with the gizmo
		auto* editX = qobject_cast<QDoubleSpinBox*>(ui->gridLayout->itemAtPosition(2, 1)->widget());
		auto xDiff = selectedObject->getPosition().x - editX->value();
		if (xDiff >= FLOAT_PRECISION || xDiff <= -FLOAT_PRECISION)
		{
			editX->setValue(selectedObject->getPosition().x);
		}

		auto* editY = qobject_cast<QDoubleSpinBox*>(ui->gridLayout->itemAtPosition(3, 1)->widget());
		auto yDiff = selectedObject->getPosition().y - editY->value();
		if (yDiff >= FLOAT_PRECISION || yDiff <= -FLOAT_PRECISION)
		{
			editY->setValue(selectedObject->getPosition().y);
		}

		auto* editZ = qobject_cast<QDoubleSpinBox*>(ui->gridLayout->itemAtPosition(4, 1)->widget());
		auto zDiff = selectedObject->getPosition().z - editZ->value();
		if (zDiff >= FLOAT_PRECISION || zDiff <= -FLOAT_PRECISION)
		{
			editZ->setValue(selectedObject->getPosition().z);
		}
	}
}