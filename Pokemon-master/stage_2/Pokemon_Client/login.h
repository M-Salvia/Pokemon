#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QUdpSocket>
#include "pokemon.h"

//客户端请求头
const uint SIGNUP=1;//注册
const uint LOGIN=2;//登录
const uint LOGOUT=3;//登出
//客户端和服务端共用的标识头
const uint ONLINEUSERS=11;//展示在线的用户
const uint ALLUSERS=12;//展示所有的用户

//服务端回应头
const uint SIGNUPOK=4;//注册成功
const uint LOGINOK=5;//登录成功
const uint LOGOUTOK=6;//登出成功
const uint NOUSER=7;//登录失败，用户不存在
const uint EXISTUSER=8;//注册失败，存在用户
const uint PWDDIFF=9;//登录失败，密码错误
const uint PKMBAGS=10;//返回宝可梦背包


namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    QList<Pkm> getPkm();
    QString getUsername();
    uint getPort();
private slots:

    void on_BtnSignUp_clicked();

    void on_BtnLogIn_clicked();

    void readPendingDatagrams();
signals:
    void loginSuccessful(const QString& username, int port);
private:
    Ui::Login *ui;

    QUdpSocket *client;
    QUdpSocket *server;

    QList<Pkm> pkms;
    QString username;
    uint port;
};

#endif // LOGIN_H
