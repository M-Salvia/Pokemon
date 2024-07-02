#include <QRandomGenerator>
#include "Enity.h"

//初步的设计就是精灵有一定机率使用技能，使用技能的频率，技能的强度，普通攻击的强度
//都和精灵的天赋有关，并随着等级的提升
//由于现在没有引入对战这一概念，所以暴击和经验这两个机制在这一阶段不实现，技能的具体效果也不展现
QString Pikachu ::Attack()
{
    // 使用 QTime 获取当前毫秒作为随机数生成器的种子
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());

    this->skills = AttackSkills.value(this->name);
    // for (const QString &skill : this->skills) {
    //     qDebug() << skill;
    // }
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0); // 返回第一个技能
        } else {
            return "No Skill";
        }
    }
}

QString Charmeleon ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = AttackSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Spearow ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = AttackSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Rattata ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = AttackSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Bulbasaur ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = BloodSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Squirtle ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = BloodSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Sandshrew ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = BloodSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Wartortle ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = BloodSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}
QString Ivysaur ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = DefenseSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}
QString Metapod ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = DefenseSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Kakuna ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = DefenseSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Vulpix ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = SpeedSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Jigglypuff ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = SpeedSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Caterpie ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = SpeedSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}

QString Weedle ::Attack()
{
    QRandomGenerator rng = QRandomGenerator(QTime::currentTime().msec());
    this->skills = SpeedSkills.value(this->name);
    uint probability = rng.generate() % 10;
    if (probability < 8)
        return NormalAttack;
    else{
        if (!this->skills.isEmpty()) {
            return this->skills.at(0);
        } else {
            return "No Skill";
        }
    }
}





