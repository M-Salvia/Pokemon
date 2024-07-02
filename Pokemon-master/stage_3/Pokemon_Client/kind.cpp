#include <QRandomGenerator>
#include "kind.h"

//构造函数
HighAttack::HighAttack(uint qualification)
{
    this->raity = RAITYKIND[qualification];

    // QRandomGenerator rng = *QRandomGenerator::global();
    QRandomGenerator rng(QTime::currentTime().msec());
    switch (qualification) // 资质越高，属性越高,当然基础的属性和精灵所属的类别有关系
    {
    //包括攻击、血量、防御、速度四个方面
    case C:
        this->SetValues(BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC);
        break;
    case B:
        this->SetValues(BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB);
        break;
    case A:
        this->SetValues(BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA);
        break;
    case S:
        this->SetValues(BASE + rng.generate() % BASES, BASE + rng.generate() % BASES, BASE + rng.generate() % BASES, BASE + rng.generate() % BASES);
        break;
    case SS:
        this->SetValues(BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS);
        break;
    case SSS:
        this->SetValues(BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS);
        break;
    default:
        break;
    }
}

void HighAttack::LevelUP()
{
    //攻击类的精灵属性增加中attack加的比较多，其他KIND类似
    this->level++;
    this->attack += 50;
    this->blood += 30;
    this->CurrentBlood = this->blood; // 升级后当前的血量回复满血
    this->defense += 30;
    this->speed += 30;
}

HighBlood::HighBlood(uint qualification)
{
    this->raity = RAITYKIND[qualification];
    QRandomGenerator rng(QTime::currentTime().msec());
    switch (qualification)
    {
    case C:
        this->SetValues(BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC);
        break;
    case B:
        this->SetValues(BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB);
        break;
    case A:
        this->SetValues(BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA);
        break;
    case S:
        this->SetValues(BASE + rng.generate() % BASES, BASE + rng.generate() % BASES, BASE + rng.generate() % BASES, BASE + rng.generate() % BASES);
        break;
    case SS:
        this->SetValues(BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS);
        break;
    case SSS:
        this->SetValues(BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS);
        break;
    default:
        break;
    }
}

void HighBlood::LevelUP()
{
    this->level++;
    this->attack += 30;
    this->blood += 50;
    this->CurrentBlood = this->blood;
    this->defense += 30;
    this->speed += 30;
}

HighDefense::HighDefense(uint qualification)
{
    this->raity = RAITYKIND[qualification];
    QRandomGenerator rng(QTime::currentTime().msec());
    switch (qualification)
    {
    case C:
        this->SetValues(BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC);
        break;
    case B:
        this->SetValues(BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB);
        break;
    case A:
        this->SetValues(BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA);
        break;
    case S:
        this->SetValues(BASE + rng.generate() % BASES, BASE + rng.generate() % BASES, BASE + rng.generate() % BASES, BASE + rng.generate() % BASES);
        break;
    case SS:
        this->SetValues(BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS);
        break;
    case SSS:
        this->SetValues(BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS);
        break;
    default:
        break;
    }
}

void HighDefense::LevelUP()
{
    this->level++;
    this->attack += 30;
    this->blood += 30;
    this->CurrentBlood = this->blood;
    this->defense += 50;
    this->speed += 30;
}

HighSpeed::HighSpeed(uint qualification)
{
    this->raity = RAITYKIND[qualification];
    QRandomGenerator rng(QTime::currentTime().msec());
    switch (qualification)
    {
    case C:
        this->SetValues(BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC, BASE + rng.generate() % BASEC);
        break;
    case B:
        this->SetValues(BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB, BASE + rng.generate() % BASEB);
        break;
    case A:
        this->SetValues(BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA, BASE + rng.generate() % BASEA);
        break;
    case S:
        this->SetValues(BASE + rng.generate() % BASES, BASE + rng.generate() % BASES, BASE + rng.generate() % BASES, BASE + rng.generate() % BASES);
        break;
    case SS:
        this->SetValues(BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS, BASE + rng.generate() % BASESS);
        break;
    case SSS:
        this->SetValues(BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS, BASE + rng.generate() % BASESSS);
        break;
    default:
        break;
    }
}

void HighSpeed::LevelUP()
{
    this->level++;
    this->attack += 30;
    this->blood += 30;
    this->CurrentBlood = this->blood;
    this->defense += 30;
    this->speed +=50;
}

