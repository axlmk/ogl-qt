#include "MainWindow.hpp"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "RectangleWindow.hpp"

MainWindow::MainWindow() : QDialog(nullptr, Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint) {
	// *** create OpenGL window

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);

	m_rectangleWindow = new RectangleWindow;
	m_rectangleWindow->setFormat(format);

	// *** create window container widget
	QWidget* container = QWidget::createWindowContainer(m_rectangleWindow);
	container->setMinimumSize(QSize(600, 400));

	// *** create the layout and insert widget container
	QVBoxLayout* vlay = new QVBoxLayout;
	vlay->setSpacing(0);
	vlay->addWidget(container);

	// now create some buttons at the bottom

	QHBoxLayout* hlay = new QHBoxLayout;

	QPushButton* changeColorBtn = new QPushButton(tr("Change colors"), this);
	connect(changeColorBtn, &QPushButton::clicked, this, &MainWindow::onChangeColors);

	QPushButton* closeBtn = new QPushButton(tr("Close"), this);
	connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

	hlay->addWidget(changeColorBtn);
	//hlay->addStretch();
	hlay->addWidget(closeBtn);

	vlay->addLayout(hlay);

	setLayout(vlay);
}


void MainWindow::onChangeColors() {
	m_rectangleWindow->update();
}