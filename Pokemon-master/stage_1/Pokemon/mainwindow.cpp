#include <QRandomGenerator>
#include <QSharedPointer>
#include <QDebug>
#include <QString>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Enity.h"
#include"pokemon.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //简单的QSS美化
    this->setStyleSheet("color: black;"
                            "background-color: #f5c70b;"
                            "selection-color: #f5c70b;"
                            "selection-background-color: blue;");

    this->InitPkm();
    this->CreatePkm();
    //调用成员函数创建一个Pokemon
}

MainWindow::~MainWindow()
{
    delete ui;
    // QSharedPointer 智能指针不需要手动delete
    // QList<QSharedPointer<Pokemon> 同理也不需要
}
void MainWindow::InitPkm()
{
    QRandomGenerator rng(QTime::currentTime().msec());
    for (int i = 0; i < 15; ++i) {
        uint qualification = rng.generate() % 6;
        switch(i)
        {
        case(0):this->allPokemons.append(QSharedPointer<Pokemon>(new class Pikachu(qualification)));break;
        case(1):this->allPokemons.append(QSharedPointer<Pokemon>(new class Charmeleon(qualification)));break;
        case(2):this->allPokemons.append(QSharedPointer<Pokemon>(new class Spearow(qualification)));break;
        case(3):this->allPokemons.append(QSharedPointer<Pokemon>(new class Rattata(qualification)));break;
        case(4):this->allPokemons.append(QSharedPointer<Pokemon>(new class Bulbasaur(qualification)));break;
        case(5):this->allPokemons.append(QSharedPointer<Pokemon>(new class Squirtle(qualification)));break;
        case(6):this->allPokemons.append(QSharedPointer<Pokemon>(new class Sandshrew(qualification)));break;
        case(7):this->allPokemons.append(QSharedPointer<Pokemon>(new class Wartortle(qualification)));break;
        case(8):this->allPokemons.append(QSharedPointer<Pokemon>(new class Ivysaur(qualification)));break;
        case(9):this->allPokemons.append(QSharedPointer<Pokemon>(new class Metapod(qualification)));break;
        case(10):this->allPokemons.append(QSharedPointer<Pokemon>(new class Kakuna(qualification)));break;
        case(11):this->allPokemons.append(QSharedPointer<Pokemon>(new class Vulpix(qualification)));break;
        case(12):this->allPokemons.append(QSharedPointer<Pokemon>(new class Jigglypuff(qualification)));break;
        case(13):this->allPokemons.append(QSharedPointer<Pokemon>(new class Caterpie(qualification)));break;
        case(14):this->allPokemons.append(QSharedPointer<Pokemon>(new class Weedle(qualification)));break;
        }
    }

}
void MainWindow::CreatePkm()
{
    QRandomGenerator rng(QTime::currentTime().msec());
    // QRandomGenerator Rng = *QRandomGenerator::global();
    uint myPokemonid = rng.generate() % 15;

    this->myPokemon = allPokemons.at(myPokemonid);
    myPokemon->setName();
    // qDebug() << allPokemons.size();
    // qDebug() << myPokemon->getName();
    if (myPokemon) {
        ui->InfoLabel->setText(myPokemon->getInformation());
        ui->AInfoLabel->hide();
    }
}

void MainWindow::on_AttackBtn_clicked()
{
    ui->AInfoLabel->show();
    if (myPokemon) {
        ui->AInfoLabel->setText(myPokemon->getName() + QString(" using ") + myPokemon->Attack());
    }
}

void MainWindow::on_LevelUpBtn_clicked()
{
    if (myPokemon) {
        myPokemon->ExperienceUp(50 * myPokemon->getLevel());
        ui->InfoLabel->setText(myPokemon->getInformation());

        if (myPokemon->getLevel() >= MAX_LEVEL) {
            ui->LevelUpBtn->setDisabled(true);//锁死按钮
        }
    }
}

void MainWindow::on_ClearBtn_clicked()
{
    myPokemon.reset();

    ui->LevelUpBtn->setDisabled(false);//因为可能这时LevelUp按钮是锁死的，先解锁

    this->CreatePkm();
}




