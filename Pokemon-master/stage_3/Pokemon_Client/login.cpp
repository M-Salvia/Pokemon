#include "login.h"
#include "ui_login.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include "mainwindow.h"
//使用UDP作为传输协议
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{

    ui->setupUi(this);
     setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
     setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    // this->setStyleSheet("color: black;"
    //                     "background-color: #f5c70b;"
    //                     "selection-color: #fff;");


    client = new QUdpSocket(this);
    server = new QUdpSocket(this);

    // 绑定 client 到端口 6666
    // if (client->bind(QHostAddress::Any, 6666)) {
    //     qDebug("Client bound to port 6666");
    // } else {
    //     qDebug("Failed to bind client to port 6666");
    //     // 处理错误
    // }

    this->port = 6000;
    bool isOk = false;

    while (!isOk) {
        if (server->bind(QHostAddress::Any, this->port)) {
            isOk = true;
            qDebug(" login is listening on port: %d", this->port);
        } else {
            this->port++;
            qDebug("Port %d is busy, trying next one", this->port);
        }
    }
}

Login::~Login()
{
    delete ui;
    //关闭端口
    if(server)
    {
        server->close ();
        delete server;
    }
    if(client)
    {
        client->close ();
        delete client;
    }
}

QList<Pkm> Login::getPkm()
{
    return this->pkms;
}

QString Login::getUsername()
{
    return this->username;
}

uint Login::getPort()
{
    return this->port;
}
void Login::readPendingDatagrams()
 {
    while (server->hasPendingDatagrams()){
    QByteArray data;
    data.resize(server->pendingDatagramSize());//调整数组大小以匹配待接受数据的大小
    QHostAddress senderIP;//发送方IP
    quint16 senderPort;//发送方端口
    server->readDatagram(data.data(), data.size(),&senderIP,&senderPort);
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    //讲UDP的数据报读入内存Ddata中，DataStream是用来解包数据的
    uint loginKind;
    QString username;
    QString pwd;
    dsIn>>loginKind >>username;

    switch (loginKind){
    case SIGNUPOK: {
        QMessageBox::information (this,"Sign Up","Sign Up Sucess");
        ui->lineEditUser->clear();
        ui->lineEditPwd->clear();
        break;
    }
    case LOGINOK: {
        QMessageBox::information (this,"Log in","Log in Success");
        ui->lineEditUser->clear();
        ui->lineEditPwd->clear();
        this->username = username;
        MainWindow *mainWindow = new MainWindow;
        connect(this, &Login::loginSuccessful, mainWindow, &MainWindow::onLoginSuccess);
        emit loginSuccessful(this->username,port);
        mainWindow->show();
        this->close();
        break;
    }
    case EXISTUSER:{
        QMessageBox::critical (this,"Sign Up Failed","Username Exist,Please Change Username.");
        ui->lineEditUser->clear();
        ui->lineEditPwd->clear();
        break;
    }
    case NOUSER:{
        QMessageBox::critical (this,"Log In Failed","No Such User,Please Login Again.");
        ui->lineEditUser->clear();
        ui->lineEditPwd->clear();
        break;
    }
    case PWDDIFF:{
        QMessageBox::critical (this,"Log In Failed","Password Don't Match',Please Login Again.");
        ui->lineEditUser->clear();
        ui->lineEditPwd->clear();
        break;
    }
    }
    }

 }

void Login::on_BtnSignUp_clicked()
{
    QString username=ui->lineEditUser->text ();
    this->username=username;
    QString pwd=ui->lineEditPwd->text ();

    if(!username.isEmpty ()&&!pwd.isEmpty ())
    {
        QByteArray temp;//SHA256加密,鉴于用户量很小，没有加盐
        temp = QCryptographicHash::hash ( pwd.toUtf8(), QCryptographicHash::Sha256 );
        pwd.clear ();
        pwd.append(temp.toHex());

        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<SIGNUP<<username<<this->port<<pwd;//发送登录类型，用户名密码以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");//服务器的IP地址是本地回环地址
        client->writeDatagram(data.data(), data.size(),serverAddress, 6666);//目标端口可以配置

        if(server->waitForReadyRead (600))//判断连接超时
            this->readPendingDatagrams ();
        else
            QMessageBox::critical (this,"Sign Up Failed","Connect Failed");

    }
    else
        QMessageBox::critical (this,"Sign Up Failed","Username And Pwd Must Not Be Empty");
}

void Login::on_BtnLogIn_clicked()
{
    QString username=ui->lineEditUser->text ();
    this->username=username;
    QString pwd=ui->lineEditPwd->text ();

    if(!username.isEmpty ()&&!pwd.isEmpty ())
    {
        // QByteArray temp;
        // temp = QCryptographicHash::hash ( pwd.toUtf8(), QCryptographicHash::Sha256 );
        // pwd.clear ();
        // pwd.setText(temp.toHex());

        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<LOGIN<<username<<this->port<<pwd;//发送登录类型，用户名密码以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6666);

        if(server->waitForReadyRead (600))//判断连接超时
            this->readPendingDatagrams ();
        else
            QMessageBox::critical (this,"Log In Failed","Connect Failed");
    }
    else
        QMessageBox::critical (this,"Log In Failed","Username And Pwd Must Not Be Empty");
}
