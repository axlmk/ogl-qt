#include <iostream>

#include <QApplication>
#include <QDebug>

#include "MainWindow.hpp"

int main(int argc, char** argv) {
	QApplication app(argc, argv);
	MainWindow mainWin;
	mainWin.show();

	int ret = mainWin.exec();
	return ret;
}