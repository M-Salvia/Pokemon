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
    // for(int i=0;i<12;i++)
    // CreatePkm(1);
    // for(int i =1;i<=15;i++)
    // {
    //     levelUpPokemon(i);
    //     levelUpPokemon(i);
    //     levelUpPokemon(i);
    // }
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
                              "pwd TEXT NOT NULL,"
                              "victory INTEGER,"
                              "defeat INTEGER"
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
    QSqlQuery query2;
    if (query2.exec("DELETE FROM pokemons")) {
        qDebug("All records from users table have been deleted.");
    } else {
        qDebug("Error occurred while deleting records from users table.");
    }
    QSqlQuery query3;
    if (query3.exec("DELETE FROM pokemonskills")) {
        qDebug("All records from users table have been deleted.");
    } else {
        qDebug("Error occurred while deleting records from users table.");
    }
}
void MainWindow::insertUserData(QSqlDatabase db, const QString &username, int port, const QString &pwd) {
    int victory = 0;
    int defeat = 0;
    QSqlQuery query(db);
    QString insertUser = "INSERT INTO Users (username, port, pwd,victory,defeat) VALUES (?, ?, ?, ?, ?)";
    query.prepare(insertUser);

    query.bindValue(0, username);
    query.bindValue(1, port);
    query.bindValue(2, pwd);
    query.bindValue(3, victory);
    query.bindValue(4, defeat);


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
        // qDebug()<<this->onlineUsers.length ();
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
    else if(kind==RATING)
    {
        QSqlQuery query;
        int victory = 0;
        int defeat = 0;
        // 直接构造SQL查询字符串
        QString sql = "SELECT victory, defeat FROM Users WHERE username = '" + username + "'";

        // 执行查询
        if (!query.exec(sql)) {
            qDebug() << "Error executing query:" << query.lastError().text();
            return;
        }

        // 检查是否有结果返回
        if (query.next()) {
            // 检索查询结果
             victory = query.value(0).toInt();
             defeat = query.value(1).toInt();
            // 在这里使用victory和defeat值
            // 例如，输出到控制台
            qDebug() << "Victory:" << victory << ", Defeat:" << defeat;
        }
        dsOut << RATING << victory << defeat;
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(), serverAddress, port);
    }
    else if(kind==FIGHT)
    {
        int gamekind;
        int currentpkmid,changepkmid;

        dsIn >> gamekind >> currentpkmid >> changepkmid;

        QSqlQuery query;
        if(gamekind == 3)
        {
            qDebug() << "1111111111111111111";
            updateExperience(query,currentpkmid,100);
        }
        else if(gamekind == 1)
        {
            //给精灵提升经验
            if(username == "robot")
            {
                updateGameRecord(query,"robot",1);
                QString UserName2 = findOwnerNameByPokemonId(currentpkmid);
                updateGameRecord(query,UserName2,0);
                updateExperience(query,currentpkmid,100);
            }
            else
            {
                updateGameRecord(query,"robot",0);
                updateGameRecord(query,username,1);
                updateExperience(query,currentpkmid,200);
            }
        }
        else if(gamekind == 2)
        {
            if(username == "robot")
            {
                updateGameRecord(query,"robot",1);
                QString UserName2 = findOwnerNameByPokemonId(currentpkmid);
                updateGameRecord(query,UserName2,0);
                updateExperience(query,currentpkmid,100);
                //用户失去changepkmid对应的精灵
                int newid = getUserIdByUsername("robot");
                transferPokemonOwnership(query,changepkmid,newid);
                if(userHasNoPokemons(query,UserName2))
                {
                    //根据用户名username查询userid
                    int userid = getUseridByUsername(query, UserName2);
                    CreatePkm(userid);
                }
            }
            else
            {
                updateGameRecord(query,"robot",0);
                updateGameRecord(query,username,1);
                updateExperience(query,currentpkmid,200);
                //用户得到changepkmid对应的精灵
                int newid = getUserIdByUsername(username);
                transferPokemonOwnership(query,changepkmid,newid);
            }
        }
        dsOut << FIGHT;
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(), serverAddress, port);
    }
}
QString MainWindow::findOwnerNameByPokemonId(int pokemonId) {
    // 准备SQL查询语句
    QString selectSql = "SELECT Users.username FROM Users "
                        "JOIN Pokemons ON Users.id = Pokemons.user_id "
                        "WHERE Pokemons.id = :pokemonId";

    // 创建QSqlQuery对象
    QSqlQuery query;

    // 使用QSqlQuery执行查询
    query.prepare(selectSql);
    query.bindValue(":pokemonId", pokemonId);

    // 执行查询
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return QString(); // 返回空字符串表示失败
    }

    // 检查是否有结果返回
    if (query.next()) {
        // 返回找到的用户名
        return query.value(0).toString();
    } else {
        // 如果没有找到宠物，返回空字符串
        qDebug() << "Pokemon not found.";
        return QString();
    }
}
void MainWindow::updateGameRecord(QSqlQuery &query, const QString &username, int result) {
    // 根据result的值决定增加哪个字段
    int userId = getUserIdByUsername(username);
    if (userId == -1) { // 假设 getUserIdByUsername 在未找到用户时返回 -1
        qDebug() << "User not found for username:" << username;
        return;
    }
    QString incrementField;
    QString updateSql;

    qDebug() << "Username:" << username << "UserID:" << userId << result;

    if (result == 1) {
        incrementField = "victory";
    } else if (result == 0) {
        incrementField = "defeat";
    } else {
        qDebug() << "Invalid result value. Use 1 for victory or 0 for defeat.";
        return;
    }

    // 构建更新语句
    updateSql = "UPDATE Users SET " + incrementField + " = " + incrementField + " + 1 WHERE id = ?";

    // 使用QSqlQuery执行更新
    query.prepare(updateSql);

    // 绑定用户ID参数
    query.bindValue(0, userId); // 注意这里使用的是位置参数

    // 执行更新语句
    if (!query.exec()) {
        qDebug() << "Error updating game record:" << query.lastError().text();
    } else {
        // 更新成功
        qDebug() << "Game record updated successfully.";
    }
}
int MainWindow::getUserIdByUsername(const QString &username) {

    // 创建QSqlQuery对象
    QSqlQuery query;

    // 准备SQL查询语句
    QString sql = "SELECT id FROM Users WHERE username = :username";
    query.prepare(sql);

    // 绑定用户名参数
    query.bindValue(":username", username);

    // 执行查询
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return -1; // 返回一个无效的ID表示失败
    }

    // 检查是否有结果返回
    if (query.next()) {
        // 返回找到的用户ID
        return query.value(0).toInt();
    } else {
        // 如果没有找到用户，返回一个无效的ID
        qDebug() << "User not found.";
        return -1;
    }
}
void MainWindow::transferPokemonOwnership(QSqlQuery &query, int changepkmid, int newOwnerId) {
    // 准备更新语句，改变特定精灵的user_id
    QString updateSql = "UPDATE Pokemons SET user_id = ? WHERE id = ?";

    // 使用QSqlQuery执行更新
    query.prepare(updateSql);
    query.bindValue(0, newOwnerId); // 新主人的用户ID
    query.bindValue(1, changepkmid); // 要转移的精灵ID

    if (!query.exec()) {
        qDebug() << "Error transferring ownership:" << query.lastError().text();
    } else {
        // 精灵所有权转移成功
        qDebug() << "Ownership transferred successfully.";
    }
}
void MainWindow::updateExperience(QSqlQuery &query, int currentpkmid,int exp) {
    // 假设每个等级所需的经验值是上一个等级的1.2倍
    double requiredExp;
    // 查询当前精灵的当前等级和经验值
    QString selectSql = "SELECT level, experience FROM Pokemons WHERE id = ?";
    QSqlQuery selectQuery = query; // 使用传入的query对象执行查询
    selectQuery.prepare(selectSql);
    selectQuery.addBindValue(currentpkmid);

    if (!selectQuery.exec()) {
        qDebug() << "Error selecting pokemon:" << selectQuery.lastError().text();
        return;
    }

    if (selectQuery.next()) {
        int currentLevel = selectQuery.value(0).toInt();
        int currentExperience = selectQuery.value(1).toInt();

        // 计算新的经验值和等级
        int newExperience = currentExperience + exp;
        int level = currentLevel;
        requiredExp = level * 60;
        // 循环直到经验值小于等于当前等级所需经验值
        while (newExperience > requiredExp && level < MAX_LEVEL) {
            newExperience -= static_cast<int>(requiredExp);
            level++;
            levelUpPokemon(currentpkmid);
            // qDebug() << requiredExp;
        }

        // 更新数据库中的经验值和等级
        QString updateExperienceSql = "UPDATE Pokemons SET experience = ?, level = ? WHERE id = ?";
        QSqlQuery updateQuery;
        updateQuery.prepare(updateExperienceSql);
        updateQuery.addBindValue(newExperience);
        updateQuery.addBindValue(level);
        updateQuery.addBindValue(currentpkmid);

        if (!updateQuery.exec()) {
            qDebug() << "Error updating experience:" << updateQuery.lastError().text();
        } else {
            // 执行其他操作，比如通知用户精灵升级了
        }
    }
}
void MainWindow::levelUpPokemon(int currentpkmid) {
    // 查询精灵的当前信息
    QString selectSql = "SELECT kind, attack, defense, speed, blood FROM Pokemons WHERE id = ?";
    QSqlQuery query;
    query.prepare(selectSql);
    query.bindValue(0, currentpkmid);
    if (!query.exec()) {
        qDebug() << "Error selecting pokemon:" << query.lastError();
        return;
    }

    if (query.next()) {
        QString kind = query.value(0).toString();
        int attack = query.value(1).toInt();
        int defense = query.value(2).toInt();
        int speed = query.value(3).toInt();
        int blood = query.value(4).toInt();

        // 根据不同的kind进行属性调整
        if (kind == "HIGH_ATTACK") {
            attack += 50;
            defense +=30;
            blood +=30;
            speed +=30;
        } else if (kind == "HIGH_BLOOD") {
            attack += 30;
            defense +=30;
            blood +=50;
            speed +=30;
        } else if (kind == "HIGH_SPEED") {
            attack += 30;
            defense +=30;
            blood +=30;
            speed +=50;
        } else if (kind == "HIGH_DEFENSE") {
            attack += 30;
            defense +=50;
            blood +=30;
            speed +=30;
        } else {
            qDebug() << "Unknown kind:" << kind;
            return;
        }

        // 更新数据库中的属性
        QString updateSql = "UPDATE Pokemons SET attack = :attack, defense = :defense, speed = :speed, blood = :blood WHERE id = :pokemonId";
        QSqlQuery updateQuery;
        updateQuery.prepare(updateSql);
        updateQuery.bindValue(":attack", attack);
        updateQuery.bindValue(":defense", defense);
        updateQuery.bindValue(":speed", speed);
        updateQuery.bindValue(":blood", blood);
        updateQuery.bindValue(":pokemonId", currentpkmid);

        if (!updateQuery.exec()) {
            qDebug() << "Error updating pokemon:" << updateQuery.lastError();
        } else {
            qDebug() << "Pokemon attributes updated successfully.";
        }
    }
}
bool MainWindow::userHasNoPokemons(QSqlQuery &query, const QString &username) {
    // 根据用户名查询用户精灵数量
    QString countPokemonsSql = "SELECT COUNT(*) FROM Pokemons WHERE user_id = (SELECT id FROM Users WHERE username = ?)";
    query.prepare(countPokemonsSql);
    query.bindValue(0, username);
    if (!query.exec()) {
        qDebug() << "Error checking pokemon count:" << query.lastError().text();
        return true; // 假设查询失败时认为用户没有精灵
    }

    if (query.next()) {
        return query.value(0).toInt() == 0;
    }
    return true;
}
int MainWindow::getUseridByUsername(QSqlQuery &query, const QString &username) {
    QString useridSql = "SELECT id FROM Users WHERE username = ?";
    query.prepare(useridSql);
    query.bindValue(0, username);
    if (!query.exec()) {
        qDebug() << "Error getting userid:" << query.lastError().text();
        return -1; // 假设返回-1表示失败
    }

    if (query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}
void MainWindow::CreatePkm(int userId)
{
    // 使用单一的 QRandomGenerator 实例
    QRandomGenerator *rng = QRandomGenerator::global();

    uint qualification = rng->bounded(100);
    if(qualification < 15)
        qualification = C;
    else if(qualification < 35)
        qualification = B;
    else if(qualification < 60)
        qualification = A;
    else if(qualification < 85)
        qualification = S;
    else if(qualification < 95)
        qualification = SS;
    else
        qualification = SSS;

    uint kind = rng->bounded(4);
    switch (kind) {
    case 0://HIGH_ATTACK
    {
        int randomIndex = rng->bounded(AttackSkills.keys().size());
        QString randomName = AttackSkills.keys().at(randomIndex);

        if(randomName == "Pikachu")
            this->pokemon = new class Pikachu(qualification);
        else if(randomName == "Charmeleon")
            this->pokemon = new class Charmeleon(qualification);
        else if(randomName == "Spearow")
            this->pokemon = new class Spearow(qualification);
        else if(randomName == "Rattata")
            this->pokemon = new class Rattata(qualification);

        Pkm *PkmA = pokemon->getPkm();
        this->insertPokemonData(*db, userId, *PkmA);

        delete PkmA;
        delete this->pokemon;
        this->pokemon = nullptr;
        break;
    }
    case 1://HIGH_BLOOD
    {
        int randomIndex = rng->bounded(BloodSkills.keys().size());
        QString randomName = BloodSkills.keys().at(randomIndex);

        if(randomName == "Bulbasaur")
            this->pokemon = new class Bulbasaur(qualification);
        else if(randomName == "Squirtle")
            this->pokemon = new class Squirtle(qualification);
        else if(randomName == "Sandshrew")
            this->pokemon = new class Sandshrew(qualification);
        else if(randomName == "Wartortle")
            this->pokemon = new class Wartortle(qualification);

        Pkm *PkmB = pokemon->getPkm();
        this->insertPokemonData(*db, userId, *PkmB);

        delete PkmB;
        delete this->pokemon;
        this->pokemon = nullptr;
        break;
    }
    case 2://HIGH_DEFENSE
    {
        int randomIndex = rng->bounded(DefenseSkills.keys().size());
        QString randomName = DefenseSkills.keys().at(randomIndex);

        if(randomName == "Ivysaur")
            this->pokemon = new class Ivysaur(qualification);
        else if(randomName == "Metapod")
            this->pokemon = new class Metapod(qualification);
        else if(randomName == "Kakuna")
            this->pokemon = new class Kakuna(qualification);

        Pkm *PkmD = pokemon->getPkm();
        this->insertPokemonData(*db, userId, *PkmD);

        delete PkmD;
        delete this->pokemon;
        this->pokemon = nullptr;
        break;
    }
    case 3://HIGH_SPEED
    {
        int randomIndex = rng->bounded(SpeedSkills.keys().size());
        QString randomName = SpeedSkills.keys().at(randomIndex);

        if(randomName == "Vulpix")
            this->pokemon = new class Vulpix(qualification);
        else if(randomName == "Jigglypuff")
            this->pokemon = new class Jigglypuff(qualification);
        else if(randomName == "Caterpie")
            this->pokemon = new class Caterpie(qualification);
        else if(randomName == "Weedle")
            this->pokemon = new class Weedle(qualification);

        Pkm *PkmS = pokemon->getPkm();
        this->insertPokemonData(*db, userId, *PkmS);

        delete PkmS;
        delete this->pokemon;
        this->pokemon = nullptr;
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


