#ifndef KIND_H
#define KIND_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "pokemon.h"
//这一层只是实现对精灵的分类，这里的类都不能实例化

class HighAttack : public Pokemon
{
    Q_OBJECT
public:
    HighAttack(uint qualification);
    virtual ~HighAttack(){}
    void setKind() override{
        this->kind = "HIGH_ATTACK";
    }
    void LevelUP() override ;
    //这一层不实现Attack的模式
signals:

public slots:
};
class HighBlood : public Pokemon
{
public:
    HighBlood(uint qualification);
    virtual~HighBlood(){}
    void setKind() override{
        this->kind = "HIGH_BLOOD";
    }
    void LevelUP() override ;
};
class HighDefense : public Pokemon
{
public:
    HighDefense(uint qualification);
    virtual ~HighDefense(){}
    void setKind() override{
        this->kind = "HIGH_DEFENSE";
    }
    void LevelUP() override ;
};
class HighSpeed : public Pokemon
{
public:
    HighSpeed(uint qualification);
    virtual ~HighSpeed(){};
    void setKind() override{
        this->kind = "HIGH_SPEED";
    }
    void LevelUP() override ;
};

#endif // KIND_H
