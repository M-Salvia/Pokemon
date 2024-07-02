#include "battlewindow.h"
#include "ui_battlewindow.h"

battlewindow::battlewindow(const QString& username,int GameKind,QWidget *parent)
    : QWidget(parent),
    username(username),
    gamekind(GameKind),
    ui(new Ui::battlewindow),
    timer(new QTimer(this)),
    isMyturn(true)
{
    ui->setupUi(this);
    client = new QUdpSocket(this);
    server = new QUdpSocket(this);

    ui->username->setText(this->username);

    qDebug() << this->gamekind;



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

    sendRequestToServer(PKMBAGS, username,0,0,0);

    foreach(Pkm pkm, pkms){
        ui->myPokemons->addItem(pkm.name);
    }
    //gamekind为0代表只是获取精灵的信息
    sendRequestToServer(PKMBAGS, "robot",0,0,0);
    foreach(Pkm pkm, Aipkms){
        ui->AIPokemons->addItem(pkm.name);
    }

    connect(timer, &QTimer::timeout, this, &battlewindow::on_TimerTimeout);

    animation1 = new QPropertyAnimation(ui->myimage, "pos");
    animation2 = new QPropertyAnimation(ui->AIimage, "pos");
    if(gamekind == 1)
    ui->label_name->setText("升级赛");
    if(gamekind == 2)
    ui->label_name->setText("决斗赛");
}

battlewindow::~battlewindow()
{
    delete ui;
    animation1 = new QPropertyAnimation(ui->myimage, "pos");
    animation2 = new QPropertyAnimation(ui->AIimage, "pos");
}
void battlewindow::sendRequestToServer(uint requestType, const QString &username,const int &gamekind,int pkmid,int changeid) {
    QByteArray data;
    QDataStream dsOut(&data, QIODevice::ReadWrite);


    if(gamekind == 0){
        qDebug() <<requestType<<username<<this->port;
        dsOut << requestType << username << this->port;
    }
    else if(gamekind == 1){
        qDebug() <<requestType<<username<<this->port<< gamekind << pkmid;
        dsOut << requestType << username << this->port << gamekind <<pkmid;
    }
    else if(gamekind == 2 || gamekind == 3){
        qDebug() <<requestType<<username<<this->port<< gamekind << pkmid << changeid;
        dsOut << requestType << username << this->port << gamekind<<pkmid << changeid;
    }


    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(data.data(), data.size(), serverAddress, 6666);
    if(server->waitForReadyRead(600) && username =="robot"){
        this->readBOTPendingDatagrams();
    }
    else if (server->waitForReadyRead(600)) {
        this->readPendingDatagrams();
    } else {
        QMessageBox::critical(this, "111Request Failed", "Connection Timeout");
    }
}
void battlewindow::on_Exit_clicked()
{
    this->close();
}
void battlewindow::readBOTPendingDatagrams()
{

    while (server->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(server->pendingDatagramSize());
        server->readDatagram(datagram.data(), datagram.size());

        QDataStream dsIn(&datagram, QIODevice::ReadOnly);
        uint dataKind;
        dsIn >> dataKind;

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

            Aipkms.clear();
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
                    for (Pkm &existingPkm : Aipkms) {
                        if (existingPkm.name == pkm.name) {
                            nameExists = true;
                            break;
                        }
                    }
                    if (nameExists) {
                        // 如果存在同名精灵，重命名新精灵
                        pkm.name += "-" + QString::number(pkm.id);
                    }

                    this->Aipkms.append(pkm);
                }
            }
            // }

        }
        if (dataKind == FIGHT)
        {
            //战斗结算完刷新一下
            sendRequestToServer(PKMBAGS, username,0,0,0);
            sendRequestToServer(PKMBAGS, "robot",0,0,0);
        }
    }
}
void battlewindow::readPendingDatagrams()
{

    while (server->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(server->pendingDatagramSize());
        server->readDatagram(datagram.data(), datagram.size());

        QDataStream dsIn(&datagram, QIODevice::ReadOnly);
        uint dataKind;
        dsIn >> dataKind;

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
        if (dataKind == FIGHT)
        {
            //战斗结算完刷新一下
            sendRequestToServer(PKMBAGS, username,0,0,0);
            sendRequestToServer(PKMBAGS, "robot",0,0,0);
        }
    }
}
void battlewindow::on_myPokemons_activated(int index)
{
    QString selectedPkmName = ui->myPokemons->itemText(index);
    QStringList nameParts = selectedPkmName.split('-');
    QString imageName = nameParts.at(0).trimmed();
    foreach (const Pkm &pkm, pkms) {
        if (pkm.name == selectedPkmName) {
            // 找到匹配的精灵，创建详细信息字符串
            // QString skillList = pkm.skills.join(", ");
            this->mypkm = pkm;
            QString details = QString("ID: %1\n"
                                      "名称: %2\n"
                                      "种类: %3\n"
                                      "稀有度: %4\n"
                                      "等级: %5\n"
                                      "经验: %6\n"
                                      "攻击力: %7\n"
                                      "血量: %8\n"
                                      "防御力: %9\n"
                                      "速度: %10\n")
                                  .arg(pkm.id)
                                  .arg(pkm.name)
                                  .arg(pkm.kind)
                                  .arg(pkm.raity)
                                  .arg(pkm.level)
                                  .arg(pkm.experience)
                                  .arg(pkm.attack)
                                  .arg(pkm.blood)
                                  .arg(pkm.defense)
                                  .arg(pkm.speed);
            ui->myinfo->setText(details);

            QString imagePath = QString(":/Pokemons/%1.png").arg(imageName);
            QPixmap pixmap(imagePath);
            ui->myimage->setPixmap(pixmap);
            ui->myimage->setScaledContents(true); // 确保图像适应标签大小
            break;
        }
    }
}
void battlewindow::on_AIPokemons_activated(int index)
{
    QString selectedPkmName = ui->AIPokemons->itemText(index);
    QStringList nameParts = selectedPkmName.split('-');
    QString imageName = nameParts.at(0).trimmed();
    foreach (const Pkm &pkm, Aipkms) {
        if (pkm.name == selectedPkmName) {
            // 找到匹配的精灵，创建详细信息字符串
            // QString skillList = pkm.skills.join(", ");
            this->botpkm = pkm;
            QString details = QString("ID: %1\n"
                                      "名称: %2\n"
                                      "种类: %3\n"
                                      "稀有度: %4\n"
                                      "等级: %5\n"
                                      "经验: %6\n"
                                      "攻击力: %7\n"
                                      "血量: %8\n"
                                      "防御力: %9\n"
                                      "速度: %10\n")
                                  .arg(pkm.id)
                                  .arg(pkm.name)
                                  .arg(pkm.kind)
                                  .arg(pkm.raity)
                                  .arg(pkm.level)
                                  .arg(pkm.experience)
                                  .arg(pkm.attack)
                                  .arg(pkm.blood)
                                  .arg(pkm.defense)
                                  .arg(pkm.speed);
            ui->Aiinfo->setText(details);

            QString imagePath = QString(":/Pokemons/%1.png").arg(imageName);
            QPixmap pixmap(imagePath);
            ui->AIimage->setPixmap(pixmap);
            ui->AIimage->setScaledContents(true); // 确保图像适应标签大小
            break;
        }
    }
}

Pokemon* battlewindow::createPokemon(const QString &name,int q) {
    if (name == "Pikachu") {
        return new class Pikachu(q);
    } else if (name == "Charmeleon") {
        return new class Charmeleon(q);
    } else if (name == "Spearow") {
        return new class Spearow(q);
    } else if (name == "Rattata") {
        return new class Rattata(q);
    } else if (name == "Bulbasaur") {
        return new class Bulbasaur(q);
    } else if (name == "Squirtle") {
        return new class Squirtle(q);
    } else if (name == "Sandshrew") {
        return new class Sandshrew(q);
    } else if (name == "Wartortle") {
        return new class Wartortle(q);
    } else if (name == "Ivysaur") {
        return new class Ivysaur(q);
    } else if (name == "Metapod") {
        return new class Metapod(q);
    } else if (name == "Kakuna") {
        return new class Kakuna(q);
    } else if (name == "Vulpix") {
        return new class Vulpix(q);
    } else if (name == "Jigglypuff") {
        return new class Jigglypuff(q);
    } else if (name == "Caterpie") {
        return new class Caterpie(q);
    } else if (name == "Weedle") {
        return new class Weedle(q);
    } else {
        return nullptr; // 如果名字不匹配任何已知的精灵，返回空指针
    }
}
void battlewindow::attack(Pkm &attacker, Pkm &defender, int q) {
    QString infoText;
    infoText += attacker.name + "'s round!\n";

    // 闪避几率
    double offset1 = static_cast<double>(defender.speed / 1500.0);
    double dodgeChance = offset1;
    // 暴击几率，假设为20%
    double offset2 = static_cast<double>((q + attacker.level) / 150.0);
    double criticalChance = 0.2 + offset2;
    // 暴击伤害倍率
    double criticalMultiplier = 1.5;
    // 生成随机数
    QRandomGenerator *rng = QRandomGenerator::global();

    // 检查是否闪避
    if (rng->generateDouble() < dodgeChance) {
        infoText += "Dodged the attack!\n";
        ui->info->setText(infoText);
        return; // 攻击被闪避
    }

    // 计算基础伤害
    int damage = attacker.attack - defender.defense / 4;
    if (damage < 1) {
        damage = 1; // 确保至少造成1点伤害
    }
    // 检查是否暴击
    if (rng->generateDouble() < criticalChance) {
        damage *= criticalMultiplier;
        qDebug() << "Critical hit!";
        infoText += "Critical hit!\n";
    }
    if (rng->generateDouble() < 0.02 * (q * 2 + attacker.level)) {
        damage *= 1.5;
        infoText += "Skill: " + attacker.skills[0] + "\n";
    } else {
        infoText += "Normal Attack\n";
    }
    damage = damage / 5;
    // 应用伤害
    if (damage < 1) {
        damage = 1; // 确保至少造成1点伤害
    }

    if(defender.CurrentBlood <= damage)
    {
        defender.CurrentBlood = 0;
    }
    else{
            defender.CurrentBlood -= damage;
    }
    // qDebug() << damage;
    // 输出攻击结果
    infoText += attacker.name + " attacks " + defender.name + " for " + QString::number(damage) + " damage.\n";
    infoText += defender.name + " has " + QString::number(defender.CurrentBlood) + " HP left.\n";
    qDebug() << attacker.name + " attacks " + defender.name + " for " + QString::number(damage) + " damage.\n";

    ui->info->setText(infoText);

    updateUI();

    if (defender.name == mypkm.name) {
        shakeLabel1(ui->myimage);
    } else {
        shakeLabel2(ui->AIimage);
    }
}

void battlewindow::on_Button_begin_clicked()
{
    mypkm.CurrentBlood = mypkm.blood;
    botpkm.CurrentBlood = botpkm.blood;
    ui->Blood_1->setText(QString::number(this->mypkm.CurrentBlood));
    ui->Blood_2->setText(QString::number(this->botpkm.CurrentBlood));
    qDebug() << mypkm.name << "VS" <<  botpkm.name;

    QStringList RAITYKIND = {"C", "B", "A", "S", "SS", "SSS"};

    for(int i = 0; i < 6; i++) {
        if(RAITYKIND[i] == mypkm.raity)
            mypkmQualification = i;
        if(RAITYKIND[i] == botpkm.raity)
            botpkmQualification = i;
    }
    isMyturn = true;
    timer->start(1000);
}
void battlewindow::on_TimerTimeout()
{
    int end = 0;
    if (mypkm.CurrentBlood > 0 && botpkm.CurrentBlood > 0) {
        if (isMyturn) {
            attack(mypkm, botpkm, mypkmQualification);
        } else {
            attack(botpkm, mypkm, botpkmQualification);
        }
        isMyturn = !isMyturn;

        if (mypkm.CurrentBlood == 0 || botpkm.CurrentBlood == 0) {
            timer->stop(); // 停止定时器
            end = 1;
        }
    } else {
        timer->stop(); // 停止定时器
        end = 1;
    }

    if(end == 1)
    {
        if(this->gamekind == 1)
        {
            //升级赛
            if(mypkm.CurrentBlood == 0)
            {
                sendRequestToServer(FIGHT,"robot",this->gamekind,this->mypkm.id,0);
            }
            if(botpkm.CurrentBlood == 0)
            {
                sendRequestToServer(FIGHT,this->username,this->gamekind,this->mypkm.id,0);
            }
        }
        else if(this->gamekind == 2)
        {
            if(mypkm.CurrentBlood == 0)
            {
                sendRequestToServer(FIGHT,"robot",this->gamekind,this->mypkm.id,this->changepkmid);
            }
            if(botpkm.CurrentBlood == 0)
            {
                sendRequestToServer(FIGHT,this->username,this->gamekind,this->mypkm.id,botpkm.id);
            }
        }
        sendRequestToServer(FIGHT,"robot",3,this->botpkm.id,botpkm.id);


        ui->myPokemons->clear();
        ui->AIPokemons->clear();
        foreach(Pkm pkm, pkms){
            ui->myPokemons->addItem(pkm.name);
        }
        foreach(Pkm pkm, Aipkms){
            ui->AIPokemons->addItem(pkm.name);
        }
    }
}
void battlewindow::updateUI() {

    ui->Blood_1->setText(QString::number(mypkm.CurrentBlood));
    ui->Blood_2->setText(QString::number(botpkm.CurrentBlood));
    if(mypkm.CurrentBlood  == 0)
    {
        if(gamekind == 2){
            ui->info->setText("You lose! Lose a Pokemon!");
            showPickPokemonDialog();
        }
        else{
            ui->info->setText("You lose!");
        }
    }
    else if(botpkm.CurrentBlood == 0)
    {
        if(gamekind == 2)
            ui->info->setText("You win! You get " + botpkm.name);
        else{
            ui->info->setText("You win!");
        }
    }

}
void battlewindow::showPickPokemonDialog() {
    // 创建一个模态对话框
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Choose a Pokemon to lose"));
    dialog->setModal(true);

    // 移除对话框的默认关闭按钮
    dialog->setWindowFlags(dialog->windowFlags() & ~Qt::WindowCloseButtonHint);

    // 创建一个列表，用于显示背包中的精灵
    QListWidget *listWidget = new QListWidget(dialog);
    listWidget->setGeometry(30, 30, 200, 120);

    // 假设背包中的精灵存储在背包列表中
    foreach (const Pkm &pkm, pkms) {
        listWidget->addItem(pkm.name);
    }

    // 创建一个按钮，用于确认选择
    QPushButton *confirmButton = new QPushButton(tr("Confirm"), dialog);
    confirmButton->setGeometry(50, 160, 100, 30);

    // 为确认按钮创建点击事件
    connect(confirmButton, &QPushButton::clicked, [this, listWidget, dialog]() {
        // 获取用户选择的精灵
        int index = listWidget->currentRow();
        if (index != -1) {
            Pkm selectedPkm = pkms[index];
            // 处理失去精灵的逻辑
            this->changepkmid = selectedPkm.id;
            // 关闭对话框
            dialog->close();
        }
        // 如果需要，可以在这里添加其他处理逻辑
    });

    // 创建一个取消按钮，但不允许关闭对话框，只是取消操作
    // QPushButton *cancelButton = new QPushButton(tr("Cancel"), dialog);
    // cancelButton->setGeometry(170, 160, 100, 30);
    // connect(cancelButton, &QPushButton::clicked, [dialog]() {
    //     // 这里可以添加取消选择的处理逻辑，但对话框不关闭
    //     // 如果需要，可以在这里添加其他处理逻辑
    // });

    // 显示对话框
    dialog->exec();
}
void battlewindow::shakeLabel1(QLabel* label) {
    QPoint originalPos = label->pos();
    animation1->setDuration(200);
    animation1->setStartValue(originalPos);
    animation1->setKeyValueAt(0.25, originalPos + QPoint(-10, 0));
    animation1->setKeyValueAt(0.75, originalPos + QPoint(10, 0));
    animation1->setEndValue(originalPos);
    animation1->setEasingCurve(QEasingCurve::InOutQuad);
    animation1->start();
}
void battlewindow::shakeLabel2(QLabel* label) {
    QPoint originalPos = label->pos();
    animation2->setDuration(200);
    animation2->setStartValue(originalPos);
    animation2->setKeyValueAt(0.25, originalPos + QPoint(-10, 0));
    animation2->setKeyValueAt(0.75, originalPos + QPoint(10, 0));
    animation2->setEndValue(originalPos);
    animation2->setEasingCurve(QEasingCurve::InOutQuad);
    animation2->start();
}




// void battlewindow::on_refresh_clicked()
// {
//     qDebug() << "hello";
//     ui->myPokemons->clear();
//     ui->AIPokemons->clear();
//     foreach(Pkm pkm, pkms){
//         ui->myPokemons->addItem(pkm.name);
//     }
//     foreach(Pkm pkm, Aipkms){
//         ui->AIPokemons->addItem(pkm.name);
//     }
// }

