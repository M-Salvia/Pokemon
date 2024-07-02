#ifndef POKEMON_H
#define POKEMON_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QtGlobal>

#define MAX_LEVEL 15

enum  KIND//四种类型
{
    HIGH_ATTACK,HIGH_BLOOD,HIGH_DEFENSE,HIGH_SPEED
};
enum Qualification{
    C,B,A,S,SS,SSS
};
enum NAME
{
    //ATTACK
    Pikachu, //皮卡丘
    Charmeleon, //火恐龙
    Spearow, //烈雀
    Rattata, //小拉达
    //BLOOD
    Bulbasaur, //妙蛙种子
    Squirtle, //杰尼龟
    Sandshrew, //穿山鼠
    Wartortle,//卡咪龟
    //DEFENSE
    Ivysaur, // 妙蛙草
    Metapod, //铁甲蛹
    Kakuna, //铁壳蛹
    //SPEED
    Vulpix,//六尾
    Jigglypuff, //胖丁
    Caterpie, //绿毛虫
    Weedle //独角虫
};
enum  SKILL//技能
{
    //高攻击力
    ThunderBolt,//雷电突击
    FireSpin,//火焰漩涡
    DiveBomb,//猛禽俯冲
    FuriousSwipes,//疯狂乱抓
    //高生命力
    PowerDrain,//强力吸取
    HydroBarrier,//水之屏障
    RolloutDefense,//滚动防御
    ShellCrush,//铁甲压制
    //高防御力
    ProtectShield,//防护罩
    HardenedSkin,//硬化皮肤
    Endure,//忍耐
    //低攻击间隔
    QuickFlame,//快速火焰
    ConfuseChant,//迷惑之音
    BugBite,//虫之咬
    PoisionSting//毒针
};

const QList<QString> RAITYKIND={
    "C",
    "B",
    "A",
    "S",
    "SS",
    "SSS"
};
struct Pkm {
    int id;
    QString name;
    QString kind;
    QString raity;
    uint level;
    uint experience;
    uint CurrentBlood;
    uint attack;
    uint blood;
    uint defense;
    uint speed;
    QList<QString> skills;

    QJsonObject toJsonObject() const {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["kind"] = kind;
        obj["raity"] = raity;
        obj["level"] =  static_cast<int>(level);
        obj["experience"] =  static_cast<int>(experience);
        obj["CurrentBlood"] =  static_cast<int>(CurrentBlood);
        obj["attack"] =  static_cast<int>(attack);
        obj["blood"] =  static_cast<int>(blood);
        obj["defense"] =  static_cast<int>(defense);
        obj["speed"] =  static_cast<int>(speed);

        QJsonArray skillsArray;
        for (const QString &skill : skills) {
            skillsArray.append(skill);
        }
        obj["skills"] = skillsArray;

        return obj;
    }
    // 从QJsonObject构造UDPPkm对象
    static Pkm toPkmObject(const QJsonObject &obj) {
        Pkm pkm;
        pkm.id = obj["id"].toInt();
        pkm.name = obj["name"].toString();
        pkm.kind = obj["kind"].toString();
        pkm.raity = obj["raity"].toString();
        pkm.level = obj["level"].toInt();
        pkm.experience = obj["experience"].toInt();
        pkm.CurrentBlood = obj["CurrentBlood"].toInt();
        pkm.attack = obj["attack"].toInt();
        pkm.blood = obj["blood"].toInt();
        pkm.defense = obj["defense"].toInt();
        pkm.speed = obj["speed"].toInt();

        QJsonArray skillsArray = obj["skills"].toArray();
        for (const QJsonValue &skillValue : skillsArray) {
            pkm.skills.append(skillValue.toString());
        }

        return pkm;
    }
};


const uint BASE = 50;
const uint BASEC = 50;
const uint BASEB = 100;
const uint BASEA = 150;
const uint BASES = 200;
const uint BASESS = 250;
const uint BASESSS = 300;

const QString NormalAttack = "Normal Attack";

class Pokemon : public QObject
{
    //QObject 是所有QT对象的基类
    Q_OBJECT
    //Q_OBJECT宏允许对象使用信号和槽机制
public:

    Pokemon():level(1),experience(0){}
    virtual ~Pokemon(){}
    //构造函数初始等级为1经验为0
    //虚析构函数保证派生类的析构函数被调用

    virtual QString Attack() = 0;
    //精灵的攻击方式是纯虚函数，必须在派生类里重写
    //攻击方式返回的是一个字符串，代表攻击方式的名字

    virtual void setName() = 0;
    virtual void setKind() = 0;
    void ExperienceUp(uint value);
    virtual void LevelUP(){}

    //这些static函数不涉及多态，使用也不需要实例化，只是工具性质
    QString getName();
    uint getLevel();
    uint getCurrentBlood();
    uint getattack();
    uint getblood();
    uint getdefense();
    uint getspeed();
    void reduceBlood(int damage);
    QString getInformation();
    Pkm* getAttr();
protected:
    QString name;     //名字
    QString kind;        //种类
    QString raity;        //稀有度
    uint level;       //等级
    uint experience;  //经验
    uint CurrentBlood;//实时血量，对决时使用
    uint attack;      //攻击属性
    uint blood;       //生命属性
    uint defense;     //防御属性
    uint speed;       //敏捷属性
    QList<QString> skills;       //技能

    void SetValues(uint baseAttack, uint baseBlood, uint baseDefense, uint baseSpeed);

signals:

public slots:
};

#endif // POKEMON_H
