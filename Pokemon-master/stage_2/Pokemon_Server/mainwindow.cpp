#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Enity.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("color: black;"
                        "background-color: #f5c70b;"
                        "selection-color: #f5c70b;"
                        "selection-background-color: blue;");
    server=new QUdpSocket(this);//创建一个UDP的套接字
    server->bind(QHostAddress::LocalHost, 6666);
    //服务端绑定端口号为6666，服务端监听并准备接受所有发往这
    connect(server, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
    //当server有新数据到达时，readyRead信号将被触发，readPendingDatagrams函数会被自动调用
    client=new QUdpSocket(this);//向客户端发消息

    db=new QSqlDatabase();
    *db=QSqlDatabase::addDatabase ("QSQLITE");
    db->setDatabaseName ("POKEMON");
    // db->setUserName ("2022211331");
    // db->setPassword ("123456");
    if(!db->open ())
    {
        qDebug()<<"Failed to SQLite";
    }
    createTable(*db);
    ui->labelPort->setText(QString("Port: %1").arg(server->localPort()));
    ui->labelDatabaseName->setText(QString("Database: %1").arg(db->databaseName()));
    // deletedb(*db);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete server;
    delete client;
    delete db;//只是删除了数据库的连接，而不是删除数据库的实体
}
//序列化PKM为JSON字符串
QString MainWindow::serializeToJson(const Pkm &pkm)
{
    QJsonObject jsonObject = pkm.toJsonObject();
    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson();
}
// 反序列化JSON字符串为Pkm结构体
Pkm MainWindow::deserializeFromJson(const QString &jsonString)
{
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonString.toUtf8()));
    QJsonObject jsonObject = jsonDoc.object();
    return Pkm::toPkmObject(jsonObject);
}
void MainWindow::createTable(QSqlDatabase db) {
    QSqlQuery query(db);

    // 创建用户表
    QString createUserTable = "CREATE TABLE IF NOT EXISTS Users ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "username TEXT NOT NULL UNIQUE,"
                              "port INTEGER NOT NULL,"
                              "pwd TEXT NOT NULL"
                              ")";
    if (!query.exec(createUserTable)) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return;
    }

    // 创建精灵基表
    QString createPokemonsTable = "CREATE TABLE IF NOT EXISTS Pokemons ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "user_id INTEGER NOT NULL,"
                                  "name TEXT NOT NULL,"
                                  "kind TEXT,"
                                  "raity TEXT,"
                                  "level INTEGER,"
                                  "experience INTEGER,"
                                  "current_blood INTEGER,"
                                  "attack INTEGER,"
                                  "blood INTEGER,"
                                  "defense INTEGER,"
                                  "speed INTEGER,"
                                  "FOREIGN KEY (user_id) REFERENCES Users (id)"
                                  ")";
    if (!query.exec(createPokemonsTable)) {
        qDebug() << "Error creating pokemons table:" << query.lastError().text();
        return;
    }

    //创建技能基表
    QString createPokemonSkillsTable = "CREATE TABLE IF NOT EXISTS PokemonSkills ("
                                       "pokemon_id INTEGER NOT NULL,"
                                       "skill TEXT NOT NULL,"
                                       "FOREIGN KEY (pokemon_id) REFERENCES Pokemons (id)"
                                       ")";
    if (!query.exec(createPokemonSkillsTable)) {
        qDebug() << "Error creating pokemon skills table:" << query.lastError().text();
        return;
    }

    // 可以选择创建索引来优化查询性能
    // 例如，为用户名称创建索引
    QString createIndex = "CREATE INDEX IF NOT EXISTS idx_username ON Users(username)";
    if (!query.exec(createIndex)) {
        qDebug() << "Error creating index:" << query.lastError().text();
    }
}
void MainWindow::deletedb(QSqlDatabase db)
{
    QSqlQuery query;
    if (query.exec("DELETE FROM users")) {
        qDebug("All records from users table have been deleted.");
    } else {
        qDebug("Error occurred while deleting records from users table.");
    }
}
void MainWindow::insertUserData(QSqlDatabase db, const QString &username, int port, const QString &pwd) {
    QSqlQuery query(db);
    QString insertUser = "INSERT INTO Users (username, port, pwd) VALUES (?, ?, ?)";
    query.prepare(insertUser);

    query.bindValue(0, username);
    query.bindValue(1, port);
    query.bindValue(2, pwd);

    if (!query.exec()) {
        qDebug() << "Error inserting user:" << query.lastError().text();
    }
}
void MainWindow::insertPokemonData(QSqlDatabase db, int userId,const Pkm& pkm) {
    QSqlQuery query(db);
    QString insertPokemon = "INSERT INTO Pokemons (user_id, name, kind, raity, level, experience, current_blood, attack, blood, defense, speed) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    query.prepare(insertPokemon);

    query.bindValue(0, userId);
    query.bindValue(1, pkm.name);
    query.bindValue(2, pkm.kind);
    query.bindValue(3, pkm.raity);
    query.bindValue(4, pkm.level);
    query.bindValue(5, pkm.experience);
    query.bindValue(6, pkm.CurrentBlood);
    query.bindValue(7, pkm.attack);
    query.bindValue(8, pkm.blood);
    query.bindValue(9, pkm.defense);
    query.bindValue(10, pkm.speed);

    if (!query.exec()) {
        qDebug() << "Error inserting pokemon:" << query.lastError().text();
    }
    int lastPokemonId = query.lastInsertId().toInt();

    insertPokemonSkillsData(db,lastPokemonId,pkm.skills);
}
void MainWindow::insertPokemonSkillsData(QSqlDatabase db, int pokemonId, const QList<QString> &skills) {
    QSqlQuery query(db);
    QString insertSkill = "INSERT INTO PokemonSkills (pokemon_id, skill) VALUES (?, ?)";
    query.prepare(insertSkill);

    foreach (const QString &skill, skills) {
        query.bindValue(0, pokemonId);
        query.bindValue(1, skill);

        if (!query.exec()) {
            qDebug() << "Error inserting skill:" << query.lastError().text();
            // 可以选择跳过当前技能或停止插入
            break;
        }
    }
}
void MainWindow::readPendingDatagrams()
{

    QByteArray dataIn;//读取数据
    dataIn.resize(server->pendingDatagramSize());
    server->readDatagram(dataIn.data(), dataIn.size());
    QDataStream dsIn(&dataIn,QIODevice::ReadWrite);
    QString username;
    QString pwd;
    uint port;
    uint kind;
    dsIn>>kind>>username>>port;

    if(kind==SIGNUP||kind==LOGIN)
    {
        dsIn >> pwd;
    }

    QByteArray dataOut;
    QDataStream dsOut(&dataOut,QIODevice::ReadWrite);
    QSqlQuery query;
    if(kind==SIGNUP)
    {
        query.exec ("select * from Users where username=\""+username+"\";");
        if(query.next ())//用户名已存在
            dsOut << EXISTUSER;
        else
        {
            insertUserData(*db,username,port,pwd);
            int userId = query.lastInsertId().toInt();
            for (int i = 0; i < 3; ++i) {
                this->CreatePkm (userId);
            }
            dsOut << SIGNUPOK;
        }
        QHostAddress clientAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(),clientAddress, port);

        //由于在UDP中服务端和客户端的界限很模糊，所以client->server只是相对的，找到IP+监听port，发送。
    }
    else if(kind == LOGIN)
    {
        // 使用参数化查询来防止SQL注入
        QString queryStr = "SELECT pwd FROM Users WHERE username = :username";
        QSqlQuery query;
        query.prepare(queryStr);
        query.bindValue(":username", username);

        if(!query.exec() || !query.next()) // 数据库中不存在此用户或查询失败
            dsOut << NOUSER;
        else
        {
            QString storedHash = query.value(0).toString(); // 假设密码哈希存储在pwd列

            // 将用户输入的密码转换为SHA256哈希
            QString inputHash = QCryptographicHash::hash(pwd.toUtf8(), QCryptographicHash::Sha256).toHex();

            // 比较输入的密码哈希和数据库中的哈希值
            if(inputHash != storedHash)
            {
                dsOut << PWDDIFF;
            }
            else
            {
                User user;
                user.username = username;
                // 确保port已经被赋值
                user.port = port;
                // 添加用户到在线用户列表
                this->onlineUsers.append(user);

                dsOut << LOGINOK << username;

            }
        }
        // 发送数据到客户端
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(), serverAddress, port);
    }
    else if(kind==LOGOUT)
    {
        User user;
        user.username =username;
        this->onlineUsers.removeOne (user);
        dsOut << LOGOUT;
        qDebug()<<username<<"Login out";
        qDebug()<<this->onlineUsers.length ();
    }
    else if(kind==ONLINEUSERS)
    {
        this->SentOnlineUsers (port);
    }
    else if(kind==ALLUSERS)
    {
        this->SentAllUsers (port);
    }
    else if(kind==PKMBAGS)
    {
        this->fetchPokemonsAndSkills(username,port);
    }
}
void MainWindow::CreatePkm(int userId)
{

        QRandomGenerator rngQualification(QTime::currentTime().msec());
        uint qualification = rngQualification() %100;
        if(qualification < 15)
            qualification=C;
        else if(qualification < 35)
            qualification=B;
        else if(qualification < 60)
            qualification=A;
        else if(qualification < 85)
            qualification=S;
        else if(qualification < 95)
            qualification=SS;
        else
            qualification=SSS;

        QRandomGenerator rngKind(QTime::currentTime().msec());
        uint kind = rngKind.generate() % 4;
        switch (kind) {
        case 0://HIGH_ATTACK
        {
            QRandomGenerator rng1(QTime::currentTime().msec());
            int randomIndex = rng1.bounded(AttackSkills.keys().size());
            QString randomName = AttackSkills.keys().at(randomIndex);

            if(randomName == "Pikachu")
                this->pokemon = new class Pikachu(qualification);
            if(randomName == "Charmeleon")
                this->pokemon = new class Charmeleon(qualification);
            if(randomName == "Spearow")
                this->pokemon = new class Spearow(qualification);
            if(randomName == "Rattata")
                this->pokemon = new class Rattata(qualification);
            Pkm *PkmA=pokemon->getPkm();

            this->insertPokemonData(*db,userId,*PkmA);

            delete PkmA;
            delete this->pokemon;
            this->pokemon=NULL;
            break;
        }
        case 1://HIGH_BLOOD
        {
            QRandomGenerator rng2(QTime::currentTime().msec());
            int randomIndex = rng2.bounded(BloodSkills.keys().size());
            QString randomName = BloodSkills.keys().at(randomIndex);

            if(randomName == "Bulbasaur")
                this->pokemon = new class Bulbasaur(qualification);
            if(randomName == "Squirtle")
                this->pokemon = new class Squirtle(qualification);
            if(randomName == "Sandshrew")
                this->pokemon = new class Sandshrew(qualification);
            if(randomName == "Wartortle")
                this->pokemon = new class Wartortle(qualification);

            Pkm *PkmB=pokemon->getPkm();
            this->insertPokemonData(*db,userId,*PkmB);

            delete PkmB;
            delete this->pokemon;
            this->pokemon=NULL;
            break;
        }
        case 2://HIGH_DEFENSE
        {
            QRandomGenerator rng3(QTime::currentTime().msec());
            int randomIndex = rng3.bounded(DefenseSkills.keys().size());
            QString randomName = DefenseSkills.keys().at(randomIndex);

            if(randomName == "Ivysaur")
                this->pokemon = new class Ivysaur(qualification);
            if(randomName == "Metapod")
                this->pokemon = new class Metapod(qualification);
            if(randomName == "Kakuna")
                this->pokemon = new class Kakuna(qualification);

            Pkm *PkmD=pokemon->getPkm();

            this->insertPokemonData(*db,userId,*PkmD);

            delete PkmD;
            delete this->pokemon;
            this->pokemon=NULL;
            break;
        }
        case 3://HIGH_SPEED
        {
            QRandomGenerator rng4(QTime::currentTime().msec());
            int randomIndex = rng4.bounded(SpeedSkills.keys().size());
            QString randomName = SpeedSkills.keys().at(randomIndex);

            if(randomName == "Vulpix")
                this->pokemon = new class Vulpix(qualification);
            if(randomName == "Jigglypuff")
                this->pokemon = new class Jigglypuff(qualification);
            if(randomName == "Caterpie")
                this->pokemon = new class Caterpie(qualification);
            if(randomName == "Weedle")
                this->pokemon = new class Weedle(qualification);

            Pkm *PkmS=pokemon->getPkm();

            this->insertPokemonData(*db,userId,*PkmS);

            delete PkmS;
            delete this->pokemon;
            this->pokemon=NULL;
            break;
        }
        default:
            break;
        }

}

void MainWindow::SentOnlineUsers(uint port)
{
    //在线用户查询
    QByteArray dataOut;
    QDataStream dsOut(&dataOut,QIODevice::WriteOnly);

    dsOut<<ONLINEUSERS;//控制命令
//    qDebug()<<"length:"<<this->onlineUsers.length ();
    //转换成Json数据发送
    QJsonArray usersJsonArray;
    for (const User &user : this->onlineUsers) {
        QJsonObject userObject;
        userObject["username"] = user.username;
        userObject["port"] = user.port;
        usersJsonArray.append(userObject);
    }

    // 将QJsonArray转换为QJsonDocument
    //QJsonDoc主要用于序列化和反序列化
    QJsonDocument usersDoc(usersJsonArray);

    // 将QJsonDocument序列化为QByteArray
    QByteArray jsonByteArray = usersDoc.toJson();

    dsOut << qint32(jsonByteArray.size()); // 写入长度，确保接收方知道读取多少字节
    dsOut.writeRawData(jsonByteArray.data(), jsonByteArray.size());
    // qDebug() << "将要发送的数据包大小(字节):" << dataOut.size();
    //发送数据报
    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(dataOut.data(), dataOut.size(), serverAddress, port);
}


void MainWindow::SentAllUsers(uint port) {
    // 玩家列表更新，玩家状态同步
    QByteArray dataOut;
    QDataStream dsOut(&dataOut, QIODevice::WriteOnly);
    dsOut << ALLUSERS; // 发送命令

    QSqlQuery query;
    query.exec("select * from Users");
    QJsonArray usersArray;

    while (query.next()) {
        QJsonObject userObject;
        userObject["username"] = query.value(1).toString();
        // 假设端口号存储为整数，使用 toInt() 转换
        userObject["port"] = query.value(2).toInt();
        usersArray.append(userObject);
    }

    QJsonDocument usersDoc(usersArray);
    // 将QJsonDocument序列化为QByteArray
    QByteArray jsonByteArray = usersDoc.toJson();

    dsOut << qint32(jsonByteArray.size()); // 写入长度
    dsOut.writeRawData(jsonByteArray.data(), jsonByteArray.size()); // 写入JSON数据
    // qDebug() << "将要发送的数据包大小(字节):" << dataOut.size();
    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(dataOut.data(), dataOut.size(), serverAddress, port);
}
void MainWindow::fetchPokemonsAndSkills(const QString& username,uint port) {
    QSqlQuery query;
    // 假设 db 是已经成功打开的数据库连接
    QString queryStr = "SELECT * FROM Pokemons WHERE user_id = (SELECT id FROM Users WHERE username = :username)";
    query.prepare(queryStr);
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error fetching pokemons:" << query.lastError().text();
        return;
    }

    QList<Pkm> pokemons;
    while (query.next()) {
        Pkm pkm;
        pkm.id = query.value("id").toInt();
        pkm.name = query.value("name").toString();
        pkm.kind = query.value("kind").toString();
        pkm.raity = query.value("raity").toString();
        pkm.level = query.value("level").toUInt();
        pkm.experience = query.value("experience").toUInt();
        pkm.CurrentBlood = query.value("current_blood").toUInt();
        pkm.attack = query.value("attack").toUInt();
        pkm.blood = query.value("blood").toUInt();
        pkm.defense = query.value("defense").toUInt();
        pkm.speed = query.value("speed").toUInt();

        // 假设您已经有一种方法来获取与精灵相关的技能
        QSqlQuery skillQuery;
        if (skillQuery.exec("SELECT skill FROM PokemonSkills WHERE pokemon_id = " + QString::number(pkm.id))) {
            QList<QString> skills;
            while (skillQuery.next()) {
                skills.append(skillQuery.value(0).toString());
            }
            pkm.skills = skills;
        }

        pokemons.append(pkm);
    }

    // 将 pokemons 转换为 JSON 并发送
    sendPokemonsAsJson(pokemons,port);
}
void MainWindow::sendPokemonsAsJson(const QList<Pkm>& pokemons, uint port) {
    // 检查 pokemons 列表是否为空，避免发送空的 JSON 数组
    if (pokemons.isEmpty()) {
        qDebug() << "没有宝可梦数据可以发送";
        return;
    }

    QJsonArray jsonPokemonsArray;
    for (const Pkm& pkm : pokemons) {
        jsonPokemonsArray.append(pkm.toJsonObject());
    }

    // 将 QJsonArray 转换为 QJsonDocument
    QJsonDocument jsonDoc(jsonPokemonsArray);

    // 序列化 QJsonDocument 为 QByteArray
    QByteArray jsonByteArray = jsonDoc.toJson();

    // 创建要发送的数据包
    QByteArray dataOut;
    QDataStream dsOut(&dataOut, QIODevice::WriteOnly);
    dsOut << PKMBAGS; // 发送请求类型
    dsOut << qint32(jsonByteArray.size()); // 写入 JSON 数据的长度
    dsOut.writeRawData(jsonByteArray.data(), jsonByteArray.size());
// qDebug() << "将要发送的数据包大小(字节):" << dataOut.size();
    // 发送数据到客户端
    QHostAddress serverAddress("127.0.0.1"); // 使用 LocalHost 常量
    client->writeDatagram(dataOut.data(), dataOut.size(), serverAddress, port);
}


