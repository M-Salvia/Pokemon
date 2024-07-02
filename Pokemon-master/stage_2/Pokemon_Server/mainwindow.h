#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtSql>
#include <QtTest>
#include <QJsonValue>
#include <QDebug>
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlDatabase>



#include"pokemon.h"
//主界面头文件，这里有ui,不过只是为了直观的展示服务端开始运行了
//kind:请求类型和应答类型，用于进行客户端和服务端的通信
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


struct User//在线用户
{
    QString username;//用户名
    int port;//端口号,由于在同一台PC上可能出现多个用户同时连接服务端的情况，所以要给每个用户都分配端口
    QString pwd;


    User toUserObject(const QJsonObject &jsonObj) {
        User user;
        user.username = jsonObj["username"].toString();
        user.port = jsonObj["port"].toInt(); // 使用 toInt() 因为端口号是 int 类型
        user.pwd = jsonObj["pwd"].toString();
        return user;
    }

    // 从 User 转换到 QJsonObject
    QJsonObject toJsonObject() const {
        QJsonObject jsonObj;
        jsonObj["username"] = username;
        jsonObj["port"] = port;
        jsonObj["pwd"] = pwd;
        return jsonObj;
    }
    bool operator==(const User &user) const
    {
        return this->username==user.username;
    }
    //用于判断用户是否存在，重定向了==

};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void CreatePkm(int userId);//随机分配小精灵

    void SentOnlineUsers(uint port);//发送在线用户
    void SentAllUsers(uint port);//发送已经注册用户
    void fetchPokemonsAndSkills(const QString& username,uint port);
    void sendPokemonsAsJson(const QList<Pkm>& pokemons,uint port);

    void createTable(QSqlDatabase db);//创建一个数据表
    void deletedb(QSqlDatabase db);//删除数据库中的数据

    void insertUserData(QSqlDatabase db, const QString &username, int port, const QString &pwd);
    void insertPokemonData(QSqlDatabase db, int userId, const Pkm& pkm);
    void insertPokemonSkillsData(QSqlDatabase db, int pokemonId, const QList<QString> &skills);

    static QString serializeToJson(const Pkm &pkm);
    static Pkm deserializeFromJson(const QString &jsonString);
private slots:
    void readPendingDatagrams();


private:
    Ui::MainWindow *ui;

    QUdpSocket *server;
    QUdpSocket *client;
    QSqlDatabase *db;
    QList<User> onlineUsers;//维护在线用户

    Pokemon *pokemon;
};

#endif // MAINWINDOW_H
