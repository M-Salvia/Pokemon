#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QMainWindow>
#include <QtNetwork>
#include <QButtonGroup> //按钮分组类头文件
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTimer>
#include <QCloseEvent>
#include <QPropertyAnimation>
#include "pokemon.h"
#include "login.h"
#include "Enity.h"
namespace Ui {
class battlewindow;
}

class battlewindow : public QWidget
{
    Q_OBJECT

public:
    explicit battlewindow(const QString& username,int GameKind,QWidget *parent = nullptr);
    ~battlewindow();

    void sendRequestToServer(uint requestType, const QString &username,const int &gamekind,int pkmid,int changeid);
    void readPendingDatagrams();
    void readBOTPendingDatagrams();
    // int compareQualification(Qualification q1, Qualification q2);
    void attack(Pkm &attacker, Pkm &defender, int q);
    Pokemon* createPokemon(const QString &name,int q);
    void updateUI();

    void showPickPokemonDialog();
private slots:
    void on_Exit_clicked();

    void on_myPokemons_activated(int index);

    void on_AIPokemons_activated(int index);

    void on_Button_begin_clicked();

     void on_TimerTimeout();

    // void on_refresh_clicked();

private:
    Ui::battlewindow *ui;


    QUdpSocket *server;
    QUdpSocket *client;

    QTimer* timer;
    int gamekind;
    QList<Pkm> pkms;
    QList<Pkm> Aipkms;
    Pkm mypkm;
    Pkm botpkm;
    QString username;
    uint port;
    bool isMyturn;
    int mypkmQualification;
    int botpkmQualification;

    int changepkmid;

    QPropertyAnimation *animation1;
    QPropertyAnimation *animation2;
    void shakeLabel1(QLabel* label);
    void shakeLabel2(QLabel* label);

};

#endif // BATTLEWINDOW_H
