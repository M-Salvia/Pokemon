#ifndef ENITY_H
#define ENITY_H
#include <QRandomGenerator>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "kind.h"
//Enity头文件主要实现的是，这里没有用数据库存精灵的数据，因为现在数据比较少
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
    Pikachu(uint qualification) : HighAttack(qualification){
        setName();
        setSkills();
    }
    virtual~Pikachu(){}

    QString Attack()  override;
    void setName() override{
        this->name = "Pikachu";
    }
    void setSkills() override{
        this->skills = AttackSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }

};
//火恐龙
class Charmeleon : public HighAttack
{
    Q_OBJECT
public:
    Charmeleon(uint qualification) : HighAttack(qualification){
        setName();
        setSkills();
    }
    virtual~Charmeleon(){}

    QString Attack()  override;
    void setName() override{
        this->name = "Charmeleon";
    }
    void setSkills() override{
        this->skills = AttackSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//烈雀
class Spearow : public HighAttack
{
    Q_OBJECT
public:
    Spearow(uint qualification) : HighAttack(qualification){
        setName();
        setSkills();
    }
    virtual~Spearow(){}

    QString Attack()  override;
    void setName() override {
        this->name = "Spearow";
    }
    void setSkills() override{
        this->skills = AttackSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//小拉达
class Rattata : public HighAttack
{
    Q_OBJECT
public:
    Rattata(uint qualification) : HighAttack(qualification){
        setName();
        setSkills();
    }
    virtual~Rattata(){}

    QString Attack() override;
    void setName() override{
        this->name = "Rattata";
    }
    void setSkills() override{
        this->skills = AttackSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//秒蛙种子
class Bulbasaur : public HighBlood
{
    Q_OBJECT
public:
    Bulbasaur(uint qualification) : HighBlood(qualification){
        setName();
        setSkills();
    }
    virtual~Bulbasaur(){}

    QString Attack() override;
    void setName() override{
        this->name = "Bulbasaur";
    }
    void setSkills() override{
        this->skills = BloodSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//杰尼龟
class Squirtle : public HighBlood
{
    Q_OBJECT
public:
    Squirtle(uint qualification) : HighBlood(qualification){
        setName();
        setSkills();
    }
    virtual~Squirtle(){}

    QString Attack() override;
    void setName() override{
        this->name = "Squirtle";
    }
    void setSkills() override{
        this->skills = BloodSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//穿山鼠
class Sandshrew : public HighBlood
{
    Q_OBJECT
public:
    Sandshrew(uint qualification) : HighBlood(qualification){
        setName();
        setSkills();
    }
    virtual~Sandshrew(){}

    QString Attack() override;
    void setName() override{
        this->name = "Sandshrew";
    }
    void setSkills() override{
        this->skills = BloodSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//卡咪龟
class Wartortle : public HighBlood
{
    Q_OBJECT
public:
    Wartortle(uint qualification) : HighBlood(qualification){
        setName();
        setSkills();
    }
    virtual~Wartortle(){}

    QString Attack() override;
    void setName() override{
        this->name = "Wartortle";
    }
    void setSkills() override{
        this->skills = BloodSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//妙蛙草
class Ivysaur : public HighDefense
{
    Q_OBJECT
public:
    Ivysaur(uint qualification) : HighDefense(qualification){
        setName();
        setSkills();
    }
    virtual~Ivysaur(){}

    QString Attack() override;
    void setName() override{
        this->name = "Ivysaur";
    }
    void setSkills() override{
        this->skills = DefenseSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//铁甲蛹
class Metapod : public HighDefense
{
    Q_OBJECT
public:
    Metapod(uint qualification) : HighDefense(qualification){
        setName();
        setSkills();
    }
    virtual~Metapod(){}

    QString Attack() override;
    void setName() override{
        this->name = "Metapod";
    }
    void setSkills() override{
        this->skills = DefenseSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//铁壳蛹
class Kakuna : public HighDefense
{
    Q_OBJECT
public:
    Kakuna(uint qualification) : HighDefense(qualification){
        setName();
        setSkills();
    }
    virtual~Kakuna(){}

    QString Attack() override;
    void setName() override{
        this->name = "Kakuna";
    }
    void setSkills() override{
        this->skills = DefenseSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//六尾
class Vulpix : public HighSpeed
{
    Q_OBJECT
public:
    Vulpix(uint qualification) : HighSpeed(qualification){
        setName();
        setSkills();
        };
    virtual~Vulpix(){}

    QString Attack() override;
    void setName() override{
        this->name = "Vulpix";
    }
    void setSkills() override{
        this->skills = SpeedSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//胖丁
class Jigglypuff : public HighSpeed
{
    Q_OBJECT
public:
    Jigglypuff(uint qualification) : HighSpeed(qualification){
        setName();
        setSkills();
    }
    virtual~Jigglypuff(){}

    QString Attack() override;
    void setName() override{
        this->name = "Jigglypuff";
    }
    void setSkills() override{
        this->skills = SpeedSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//绿毛虫
class Caterpie : public HighSpeed
{
    Q_OBJECT
public:
    Caterpie(uint qualification) : HighSpeed(qualification){
        setName();
        setSkills();
    }
    virtual~Caterpie(){}

    QString Attack() override;
    void setName() override{
        this->name = "Caterpie";
    }
    void setSkills() override{
        this->skills = SpeedSkills.value(this->name);
    }

    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};
//独角虫
class Weedle : public HighSpeed
{
    Q_OBJECT
public:
    Weedle(uint qualification) : HighSpeed(qualification){
        setName();
        setSkills();
    }
    virtual~Weedle(){}

    QString Attack() override;
    void setName() override{
        this->name = "Weedle";
    }
    void setSkills() override{
        this->skills = SpeedSkills.value(this->name);
    }
    Pkm* getPkm() override {
        Pkm *attr=new Pkm();
        attr->name=this->name;
        attr->level=this->level;
        attr->experience=this->experience;
        attr->attack=this->attack;
        attr->blood=this->blood;
        attr->defense=this->defense;
        attr->speed=this->speed;
        attr->kind=this->kind;
        attr->raity=this->raity;
        attr->skills=this->skills;
        return attr;
    }
};



#endif // ENITY_H
