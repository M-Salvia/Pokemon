#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QButtonGroup> //按钮分组类头文件
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTimer>
#include <QCloseEvent>
#include "pokemon.h"
#include "login.h"


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
};
class Login;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);


    ~MainWindow();

    void clearUsersList(QList<User*> *usersList);
    static QString serializeToJson(const Pkm &pkm);
    static Pkm deserializeFromJson(const QString &jsonString);

    void updateUserInfo();
    void readPendingDatagrams();
    void sendRequestToServer(uint requestType, const QString &username);

public slots:
    void onLoginSuccess(const QString& username, int port);
private slots:
     void on_BtnLoginOut_clicked();

     void on_onlineUsersComboBox_activated(int index);

     void on_allUsersComboBox_activated(int index);

     void on_BtnRefresh_clicked();

     void on_BtnHomePage_clicked();

     void on_CurrentPkms_activated(int index);
 protected:
     void closeEvent(QCloseEvent *event) override {
         qDebug("窗口关闭，发送LOGOUT信号");
         //关闭窗口也会执行登出
         QByteArray data;
         QDataStream dsOut(&data,QIODevice::ReadWrite);
         dsOut<<LOGOUT<<this->username<<this->port;
         QHostAddress serverAddress = QHostAddress("127.0.0.1");
         client->writeDatagram(data.data(), data.size(),serverAddress, 6666);
         event->accept();  // 允许窗口关闭
     }
 private:
    Ui::MainWindow *ui;

    QUdpSocket *server;
    QUdpSocket *client;

    QList<Pkm> pkms;

    QString username;
    uint port;
    QList<User*> onlineUsers;
    QList<User*> allUsers;
};

#endif // MAINWINDOW_H
