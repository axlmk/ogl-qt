#ifndef MainWindow_H
#define MainWindow_H

#include <QDialog>

class RectangleWindow;

/*! A simple test dialog for an embedded OpenGL window. */
class MainWindow : public QDialog {
	Q_OBJECT
public:
	MainWindow();

private slots:
	void onChangeColors();

private:
	RectangleWindow* m_rectangleWindow;
};

#endif // MainWindow_H