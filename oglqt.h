#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_oglqt.h"

class oglqt : public QMainWindow
{
    Q_OBJECT

public:
    oglqt(QWidget *parent = nullptr);
    ~oglqt();

private:
    Ui::oglqtClass ui;
};
