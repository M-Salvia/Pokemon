#include <QApplication>
#include<QDebug>

#include "mainwindow.h"
#include "pokemon.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
