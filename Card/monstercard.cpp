#include "monstercard.h"

MonsterCard::MonsterCard()
    : Card()
{
    this->_attack = 0;
    this->_hasSkill = false;
}

MonsterCard::MonsterCard(QString &name, QString &description, const int consume, int attack, bool hasSkill)
    : Card(name, description, consume)
{
    this->_attack = attack;
    this->_hasSkill = hasSkill;
}

MonsterCard::MonsterCard(const MonsterCard & other)
    : Card(other)
{
    this->_attack = other._attack;
    this->_hasSkill = other._hasSkill;
}

int MonsterCard::skill()
{
    return -1;
}

//MonsterCard::~MonsterCard()
//{
//    if (this->_arms != nullptr)
//        delete _arms;
//}

int MonsterCard::getCategory() const
{
    return this->_category;
}

MonsterCard &MonsterCard::operator =(const MonsterCard & other)
{
    if(this == &other)
        return *this;
    Card::operator =(other);
    this->_attack = other._attack;
    this->_hasSkill = other._hasSkill;
    return *this;
}

void MonsterCard::operator +(ArmsCard &arm)
{
    this->_arms = &arm;
    arm._owner = this;
    arm.skill();
}

MonsterCard &MonsterCard::operator -(MonsterCard & enemy)
{
    if(this->_arms != nullptr)
    {
        *this->_arms - enemy;
    }
    else
    {
        int a_value, b_value;
        if ( enemy._arms != nullptr )
        {
            a_value = this->_attack - enemy._arms->_attackBuff;
            b_value = enemy._arms->_attackBuff - this->_attack;
            this->_attack = a_value;
            enemy._arms->_attackBuff = b_value;
        }
        else
        {
            a_value = this->_attack - enemy._attack;
            b_value = enemy._attack - this->_attack;
            this->_attack = a_value;
            enemy._attack = b_value;
        }
    }
    return *this;
}

QDataStream &operator<<(QDataStream & stream, const MonsterCard & data)
{
    stream << (const Card &) data;
    stream << data._attack << data._hasSkill;
    return stream;
}

QDataStream &operator>>(QDataStream & stream, MonsterCard & card)
{
    stream >> (Card &) card;
    stream >> card._attack >> card._hasSkill;
    return stream;
}


