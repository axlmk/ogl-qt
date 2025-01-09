#pragma once

#include <QDialog>

class RenderingWindow;

class MainWindow : public QDialog {
	Q_OBJECT
public:
	MainWindow();

private slots:
	void onChangeColors();

private:
	RenderingWindow *m_rectangleWindow;
};