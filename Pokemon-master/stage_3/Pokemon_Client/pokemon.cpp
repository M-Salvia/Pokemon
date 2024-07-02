#include "pokemon.h"

void Pokemon::ExperienceUp(uint value)
{
    //经验是累积的，等级是经验到某个数值后的一个记号
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
uint Pokemon::getCurrentBlood()
{
    return this->CurrentBlood;
}
uint Pokemon::getattack()
{
    return this->attack;
}
uint Pokemon::getblood()
{
     return this->blood;
}
uint Pokemon::getdefense()
{
    return this->defense;
}
uint Pokemon::getspeed()
{
    return this->speed;
}
void Pokemon::reduceBlood(int damage)
{
    this->CurrentBlood = this->CurrentBlood - damage;
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


