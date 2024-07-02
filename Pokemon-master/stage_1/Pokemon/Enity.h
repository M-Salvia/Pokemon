#ifndef ENITY_H
#define ENITY_H
#include <QRandomGenerator>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "kind.h"
//暂不用数据库
const QHash<QString, QList<QString>> AttackSkills = {
    {"Pikachu", QList<QString>{"Thunder Bolt" }},
    {"Charmeleon", QList<QString>{"Fire Spin" }},
    {"Spearow", QList<QString>{"Dive Bomb"}},
    {"Rattata", QList<QString>{"Furious Swipes"}}
};

const QHash<QString, QList<QString>> BloodSkills = {
    {"Bulbasaur", QList<QString>{"Power Drain" }},
    {"Squirtle", QList<QString>{"Hydro Barrie" }},
    {"Sandshrew", QList<QString>{"Rollout Defense"}},
    {"Wartortle", QList<QString>{"Shell Crush"}},
    };

const QHash<QString, QList<QString>> DefenseSkills = {
    {"Ivysaur", QList<QString>{"Protect Shield" }},
    {"Metapod", QList<QString>{"Hardened Skin" }},
    {"Kakuna", QList<QString>{"Endure"}}
};

const QHash<QString, QList<QString>> SpeedSkills = {
    {"Vulpix", QList<QString>{"Quick Flame" }},
    {"Jigglypuff", QList<QString>{"Confuse Chant" }},
    {"Caterpie", QList<QString>{"Bug Bite"}},
    {"Weedle", QList<QString>{"Poison Sting"}}
};

//这些类都不是抽象类，可以实例化
//皮卡丘
class Pikachu : public HighAttack
{
    Q_OBJECT
public:
    Pikachu(uint qualification) : HighAttack(qualification){}
    virtual~Pikachu(){}

    QString Attack()  override;
    void setName() override{
        this->name = "Pikachu";
    }

};
//火恐龙
class Charmeleon : public HighAttack
{
    Q_OBJECT
public:
    Charmeleon(uint qualification) : HighAttack(qualification){}
    virtual~Charmeleon(){}

    QString Attack()  override;
    void setName() override{
        this->name = "Charmeleon";
    }
};
//烈雀
class Spearow : public HighAttack
{
    Q_OBJECT
public:
    Spearow(uint qualification) : HighAttack(qualification){}
    virtual~Spearow(){}

    QString Attack()  override;
    void setName() override {
        this->name = "Spearow";
    }
};
//小拉达
class Rattata : public HighAttack
{
    Q_OBJECT
public:
   Rattata(uint qualification) : HighAttack(qualification){}
    virtual~Rattata(){}

    QString Attack() override;
    void setName() override{
        this->name = "Rattata";
    }
};
//秒蛙种子
class Bulbasaur : public HighBlood
{
    Q_OBJECT
public:
    Bulbasaur(uint qualification) : HighBlood(qualification){}
    virtual~Bulbasaur(){}

    QString Attack() override;
    void setName() override{
        this->name = "Bulbasaur";
    }
};
//杰尼龟
class Squirtle : public HighBlood
{
    Q_OBJECT
public:
    Squirtle(uint qualification) : HighBlood(qualification){}
    virtual~Squirtle(){}

    QString Attack() override;
    void setName() override{
        this->name = "Squirtle";
    }
};
//穿山鼠
class Sandshrew : public HighBlood
{
    Q_OBJECT
public:
    Sandshrew(uint qualification) : HighBlood(qualification){}
    virtual~Sandshrew(){}

    QString Attack() override;
    void setName() override{
        this->name = "Sandshrew";
    }
};
//卡咪龟
class Wartortle : public HighBlood
{
    Q_OBJECT
public:
    Wartortle(uint qualification) : HighBlood(qualification){}
    virtual~Wartortle(){}

    QString Attack() override;
    void setName() override{
        this->name = "Wartortle";
    }
};
//妙蛙草
class Ivysaur : public HighDefense
{
    Q_OBJECT
public:
    Ivysaur(uint qualification) : HighDefense(qualification){}
    virtual~Ivysaur(){}

    QString Attack() override;
    void setName() override{
        this->name = "Ivysaur";
    }
};
//铁甲蛹
class Metapod : public HighDefense
{
    Q_OBJECT
public:
    Metapod(uint qualification) : HighDefense(qualification){}
    virtual~Metapod(){}

    QString Attack() override;
    void setName() override{
        this->name = "Metapod";
    }
};
//铁壳蛹
class Kakuna : public HighDefense
{
    Q_OBJECT
public:
    Kakuna(uint qualification) : HighDefense(qualification){}
    virtual~Kakuna(){}

    QString Attack() override;
    void setName() override{
        this->name = "Kakuna";
    }
};
//六尾
class Vulpix : public HighSpeed
{
    Q_OBJECT
public:
    Vulpix(uint qualification) : HighSpeed(qualification){};
    virtual~Vulpix(){}

    QString Attack() override;
    void setName() override{
        this->name = "Vulpix";
    }
};
//胖丁
class Jigglypuff : public HighSpeed
{
    Q_OBJECT
public:
    Jigglypuff(uint qualification) : HighSpeed(qualification){}
    virtual~Jigglypuff(){}

    QString Attack() override;
    void setName() override{
        this->name = "Jigglypuff";
    }
};
//绿毛虫
class Caterpie : public HighSpeed
{
    Q_OBJECT
public:
    Caterpie(uint qualification) : HighSpeed(qualification){}
    virtual~Caterpie(){}

    QString Attack() override;
    void setName() override{
        this->name = "Caterpie";
    }
};
//独角虫
class Weedle : public HighSpeed
{
    Q_OBJECT
public:
    Weedle(uint qualification) : HighSpeed(qualification){}
    virtual~Weedle(){}

    QString Attack() override;
    void setName() override{
        this->name = "Weedle";
    }
};



#endif // ENITY_H
