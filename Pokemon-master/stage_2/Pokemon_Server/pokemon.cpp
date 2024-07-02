#include "pokemon.h"

void Pokemon::ExperienceUp(uint value)
{
    this->experience+=value;

    uint levelUpValue=50*this->level*(this->level+1)/2;//未升级前的升级上限
    while(this->level < MAX_LEVEL && this->experience >= levelUpValue)//while循环是为了连升多级
    {
        this->LevelUP();
        levelUpValue+=50*this->level;
    }
}

QString Pokemon::getName()
{
    return this->name;
}

uint Pokemon::getLevel()
{
    return this->level;
}
//在服务端的ui不需要展示信息，这个函数其实是用不到的
QString Pokemon::getInformation()
{
    QString information,temp;
    information=information+"name:"+this->name;
    information=information+"\nkind:"+this->kind;
    information=information+"\nraity:"+this->raity;
    information=information+"\nlevel:"+temp.setNum (level);
    information=information+"\nexperience:"+temp.setNum (experience);
    information=information+"\nattack:"+temp.setNum (attack);
    information=information+"\nblood:"+temp.setNum (blood);
    information=information+"\nCurrentBlood:"+temp.setNum (CurrentBlood);
    information=information+"\ndefense:"+temp.setNum (defense);
    information=information+"\nspeed:"+temp.setNum (speed);
    information+='\n';

    return information;
}

void Pokemon::SetValues(uint baseAttack, uint baseBlood, uint baseDefense, uint baseSpeed)
{
    this->setKind();
    this->attack=baseAttack;
    this->blood=baseBlood;
    this->CurrentBlood=baseBlood;
    this->defense=baseDefense;
    this->speed=baseSpeed;
}

