#include "home.h"
#include "ui_home.h"
#include "mainwindow.h"
home::home( const QString& username, QWidget *parent)
    : QWidget(parent),
    username(username),
    ui(new Ui::home)
{
    ui->setupUi(this);


    client = new QUdpSocket(this);
    server = new QUdpSocket(this);


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
    ui->lineEditUsername->setText(username);
    ui->lineEditPort->setText(QString::number(port));
    sendRequestToServer(RATING,username);
    sendRequestToServer(PKMBAGS, username);
    int low_pkm = 0;
    int high_pkm = 0;
    foreach(Pkm pkm, pkms){
        ui->comboBox->addItem(pkm.name);
        low_pkm++;
        if(pkm.level >= 15)
            high_pkm++;
    }
    QPixmap bronzePixmap(":/label/Bronze.png");
    QPixmap silverPixmap(":/label/Silver.png");
    QPixmap goldPixmap(":/label/Gold.png");

    if (!bronzePixmap.isNull() && !silverPixmap.isNull() && !goldPixmap.isNull()) {
        if (low_pkm < 6)
            ui->label_low->setPixmap(bronzePixmap.scaled(50, 50));
        else if (low_pkm < 11)
            ui->label_low->setPixmap(silverPixmap.scaled(50, 50));
        else
            ui->label_low->setPixmap(goldPixmap.scaled(50, 50));

        if (high_pkm < 3)
            ui->label_high->setPixmap(bronzePixmap.scaled(50, 50));
        else if (high_pkm < 10)
            ui->label_high->setPixmap(silverPixmap.scaled(50, 50));
        else
            ui->label_high->setPixmap(goldPixmap.scaled(50, 50));
    } else {
        qDebug() << "Failed to load one or more images.";
    }
}


home::~home()
{
    delete ui;
}
void home::sendRequestToServer(uint requestType, const QString &username) {
    QByteArray data;
    QDataStream dsOut(&data, QIODevice::ReadWrite);

    // qDebug() <<requestType<<username<<this->port;
    dsOut << requestType << username << this->port;

    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(data.data(), data.size(), serverAddress, 6666);

    if (server->waitForReadyRead(600)) {
        this->readPendingDatagrams();
    } else {
        QMessageBox::critical(this, "Request Failed", "Connection Timeout");
    }
}
void home::readPendingDatagrams()
{

    int victory = 0;
    int defeat = 0;
    while (server->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(server->pendingDatagramSize());
        server->readDatagram(datagram.data(), datagram.size());

        QDataStream dsIn(&datagram, QIODevice::ReadOnly);
        uint dataKind;
        dsIn >> dataKind;

        if (dataKind == RATING)
        {
            dsIn >> victory >> defeat;
            double a = static_cast<double>(victory) / (victory + defeat);
            QString rate = QString::number(a * 100,'f', 1) + "%";
            ui->lineEditWinrate->setText(rate);
        }
        if (dataKind == PKMBAGS)
        {
        qint32 jsonSize;
        dsIn >> jsonSize;

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
void home::on_comboBox_activated(int index)
{
    QString selectedPkmName = ui->comboBox->itemText(index);
    QStringList nameParts = selectedPkmName.split('-');
    QString imageName = nameParts.at(0).trimmed();
    foreach (const Pkm &pkm, pkms) {
        if (pkm.name == selectedPkmName) {
            // 找到匹配的精灵，创建详细信息字符串
            QString skillList = pkm.skills.join(", ");
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
            ui->label->setText(details);


            QString imagePath = QString(":/Pokemons/%1.png").arg(imageName);
            QPixmap pixmap(imagePath);
            ui->Image->setPixmap(pixmap);
            ui->Image->setScaledContents(true); // 确保图像适应标签大小
            break;
        }
    }
}

