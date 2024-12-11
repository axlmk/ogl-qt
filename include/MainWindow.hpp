#pragma once

#include <QDialog>

class RectangleWindow;

class MainWindow : public QDialog {
	Q_OBJECT
public:
	MainWindow();

private slots:
	void onChangeColors();

private:
	RectangleWindow* m_rectangleWindow;
};