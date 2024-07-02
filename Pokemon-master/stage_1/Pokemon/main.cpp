#include <QApplication>
#include<QDebug>

#include "mainwindow.h"
#include"Enity.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
