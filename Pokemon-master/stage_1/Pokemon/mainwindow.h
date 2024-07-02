#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Enity.h"

namespace Ui {
    class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);//防止隐式转换的出现
    ~MainWindow();

    void InitPkm();
    void CreatePkm();//随机创造一个Pokemon
private slots:

    void on_AttackBtn_clicked();

    void on_LevelUpBtn_clicked();

    void on_ClearBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSharedPointer<Pokemon> myPokemon;
    QList<QSharedPointer<Pokemon>> allPokemons;
};

#endif // MAINWINDOW_H
