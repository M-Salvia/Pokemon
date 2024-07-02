#include <QApplication>
#include <QDebug>

#include <QWidget>

#include "mainwindow.h"
#include "login.h"
#include"pokemon.h"
#include "Enity.h""

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    Login login;
    login.show();
    return  a.exec ();
    // return 0;
    // if(login.exec ()==QDialog::Accepted)
    // {
    //     return a.exec ();
    // }
    // else
    //     return 0;
}
