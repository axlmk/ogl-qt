#include "oglqt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    oglqt w;
    w.show();
    return a.exec();
}
