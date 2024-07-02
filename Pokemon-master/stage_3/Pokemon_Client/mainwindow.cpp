#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "home.h"
#include <QDebug>
#include <QTimer>

//构造函数、析构函数
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //server接受数据，client发送数据
    client = new QUdpSocket(this);
    server = new QUdpSocket(this);

    // 绑定 client 到端口 6666


    this->port = 6000; // 服务端开始监听的端口号
    bool isOk = false;

    // 寻找一个空闲端口让 server 监听
    while (!isOk) {
        if (server->bind(QHostAddress::Any, this->port)) {
            isOk = true;
            qDebug("Mainwindown is listening on port: %d", this->port);
        } else {
            this->port++;
            qDebug("Port %d is busy, trying next one", this->port);
        }
    }
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000); // 设置计时器间隔为1000毫秒（1秒）
    connect(timer, &QTimer::timeout, this, &MainWindow::updateUserInfo); // 连接信号和槽

    // 启动计时器
    timer->start();
    // if (client->bind(QHostAddress::Any, 6666)) {
    //     qDebug("Client bound to port 6666");
    // } else {
    //     qDebug("Failed to bind client to port 6666");
    //     // 处理错误
    // }

}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
    delete client;
}

//工具函数
void MainWindow::onLoginSuccess(const QString &username, int port) {
    this->username = username;
}
// 序列化UDPPkm结构体为JSON字符串
QString MainWindow::serializeToJson(const Pkm &pkm)
{
    QJsonObject jsonObject = pkm.toJsonObject();
    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}
// 反序列化JSON字符串Pkm结构体
Pkm MainWindow::deserializeFromJson(const QString &jsonString)
{
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonString.toUtf8()));
    QJsonObject jsonObject = jsonDoc.object();
    return Pkm::toPkmObject(jsonObject);
}
void MainWindow::clearUsersList(QList<User*>* usersList) {
    // 你的清空用户列表的实现代码
    qDeleteAll(*usersList); // 清空 QList 并删除 User 对象
    usersList->clear();     // 清空 QList
}

//通信函数
void MainWindow::updateUserInfo() {
    sendRequestToServer(ONLINEUSERS, username);
    sendRequestToServer(ALLUSERS, username);
    sendRequestToServer(PKMBAGS, username);
}
void MainWindow::sendRequestToServer(uint requestType, const QString &username) {
    QByteArray data;
    QDataStream dsOut(&data, QIODevice::ReadWrite);

    // qDebug() <<requestType<<username<<this->port;
    dsOut << requestType << username << this->port;

    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(data.data(), data.size(), serverAddress, 6666);

    if (server->waitForReadyRead(3000)) {
        this->readPendingDatagrams();
    } else {
        QMessageBox::critical(this, "Request Failed", "Connection Timeout");
    }
}
void MainWindow::readPendingDatagrams()
{
    while (server->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(server->pendingDatagramSize());
        server->readDatagram(datagram.data(), datagram.size());

        QDataStream dsIn(&datagram, QIODevice::ReadOnly);
        uint dataKind;
        dsIn >> dataKind;
        qint32 jsonSize;
        dsIn >> jsonSize;

        // qDebug() << "接收到的数据报文大小:" << datagram.size();
        // qDebug() << "发送端传来的 JSON 数据大小:" << jsonSize;

        if (jsonSize <= 0 || jsonSize > datagram.size() - sizeof(uint) - sizeof(qint32)) {
            qDebug() << "数据长度不正确";
            continue;
        }

        // 提取 JSON 数据
        QByteArray jsonData = datagram.mid(sizeof(uint) + sizeof(qint32), jsonSize);
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "JSON解析错误:" << parseError.errorString();
            continue;
        }

        // qDebug() << "JSON数据:";
        // qDebug() << jsonDoc.toJson(QJsonDocument::Indented);

        // 根据 dataKind 处理不同的数据类型
        if (dataKind == ONLINEUSERS || dataKind == ALLUSERS) {
            QList<User*> *usersList = (dataKind == ONLINEUSERS) ? &this->onlineUsers : &this->allUsers;
            clearUsersList(usersList);

            // 解析用户数据
            QJsonArray usersArray = jsonDoc.array();
            for (const QJsonValue &value : usersArray) {
                if (value.isObject()) {
                    QJsonObject userObj = value.toObject();
                    User *user = new User;
                    user->username = userObj["username"].toString();
                    user->port = userObj["port"].toInt();
                    usersList->append(user);
                }
            }
        }
        if (dataKind == PKMBAGS)
        {
             pkms.clear();
            QJsonArray pokemonsArray = jsonDoc.array();
            for (const QJsonValue &value : pokemonsArray) {
                if (value.isObject()) {
                    QJsonObject pkmObj = value.toObject();
                    Pkm pkm;
                    pkm.id = pkmObj["id"].toInt();
                    pkm.name = pkmObj["name"].toString();
                    pkm.kind = pkmObj["kind"].toString();
                    pkm.raity = pkmObj["raity"].toString();
                    pkm.level = pkmObj["level"].toInt();
                    pkm.experience = pkmObj["experience"].toInt();
                    pkm.CurrentBlood = pkmObj["CurrentBlood"].toInt();
                    pkm.attack = pkmObj["attack"].toInt();
                    pkm.blood = pkmObj["blood"].toInt();
                    pkm.defense = pkmObj["defense"].toInt();
                    pkm.speed = pkmObj["speed"].toInt();

                    // 假设 JSON 中也包含了技能数组
                    QJsonArray skillsArray = pkmObj["skills"].toArray();
                    for (const QJsonValue &skillValue : skillsArray) {
                        pkm.skills.append(skillValue.toString());
                    }
                    bool nameExists = false;
                    for (Pkm &existingPkm : pkms) {
                        if (existingPkm.name == pkm.name) {
                            nameExists = true;
                            break;
                        }
                    }

                    if (nameExists) {
                        // 如果存在同名精灵，重命名新精灵
                        pkm.name += "-" + QString::number(pkm.id);
                    }

                    this->pkms.append(pkm);
                }
            }
        }
    }
}


//槽函数
void MainWindow::on_BtnHomePage_clicked()
{
    // home *userHome = new home(this, this->username);
    home *userHome = new home(this->username,  nullptr);
    userHome->show();
}
// MainWindow 中的登出按钮槽函数
void MainWindow::on_BtnLoginOut_clicked()
{
    QByteArray data;
    QDataStream dsOut(&data,QIODevice::ReadWrite);
    dsOut<<LOGOUT<<this->username<<this->port;
    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(data.data(), data.size(),serverAddress, 6666);
    this->close();
    Login *loginWindow = new Login(this); // 假设 Login 继承自 QDialog
    loginWindow->show();
}
void MainWindow::on_BtnRefresh_clicked()
{
    ui->onlineUsersComboBox->clear();
    ui->allUsersComboBox->clear();
    ui->CurrentPkms->clear();
    foreach(User *user, onlineUsers) {
        qDebug() << "Online User:" << user->username;
        ui->onlineUsersComboBox->addItem(user->username);
    }
    foreach(User *user, allUsers) {
        qDebug() << "All User:" << user->username;
        ui->allUsersComboBox->addItem(user->username);
    }
    foreach(Pkm pkm, pkms){
        qDebug() << "Pokemon:" << pkm.name << " - ID:" << pkm.id;
        ui->CurrentPkms->addItem(pkm.name);
    }
}

void MainWindow::on_onlineUsersComboBox_activated(int index)
{
    QString selectedUserName = ui->onlineUsersComboBox->itemText(index);
    home *userHome = new home(selectedUserName,  nullptr);
    userHome->show();
}
void MainWindow::on_allUsersComboBox_activated(int index)
{
    QString selectedUserName = ui->allUsersComboBox->itemText(index);
    home *userHome = new home(selectedUserName,  nullptr);
    userHome->show();
}

void MainWindow::on_CurrentPkms_activated(int index)
{
     QString selectedPkmName = ui->CurrentPkms->itemText(index);

    foreach (const Pkm &pkm, pkms) {
        if (pkm.name == selectedPkmName) {
            QString skillList = pkm.skills.join(", ");
            // 找到匹配的精灵，创建详细信息字符串
            QString details = QString("ID: %1\n"
                                      "名称: %2\n"
                                      "种类: %3\n"
                                      "稀有度: %4\n"
                                      "等级: %5\n"
                                      "经验: %6\n"
                                      "攻击力: %7\n"
                                      "血量: %8\n"
                                      "防御力: %9\n"
                                      "速度: %10\n"
                                      "技能列表: %11")
                                  .arg(pkm.id)
                                  .arg(pkm.name)
                                  .arg(pkm.kind)
                                  .arg(pkm.raity)
                                  .arg(pkm.level)
                                  .arg(pkm.experience)
                                  .arg(pkm.attack)
                                  .arg(pkm.blood)
                                  .arg(pkm.defense)
                                  .arg(pkm.speed)
                                  .arg(skillList);
            ui->labelInfo->setText(details);
            break;
        }
    }
}


void MainWindow::on_Battle1_clicked()
{
    //升级赛
    battleWindow = new battlewindow(username,1);
    battleWindow->show();
}
void MainWindow::on_Batttle_2_clicked()
{
    //决斗赛
    battleWindow = new battlewindow(username,2);
    battleWindow->show();
}
