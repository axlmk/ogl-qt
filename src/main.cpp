#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	MainWindow mainWin;
	mainWin.show();

	return mainWin.exec();
}