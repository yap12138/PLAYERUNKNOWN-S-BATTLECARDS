#include "monstercard.h"

MonsterCard::MonsterCard()
    : Card()
{
    this->_attack = 0;
    this->_hasSkill = false;
    this->_arms = nullptr;
}

MonsterCard::MonsterCard(QString &name, QString &description, const int consume, int attack, bool hasSkill)
    : Card(name, description, consume)
{
    this->_attack = attack;
    this->_hasSkill = hasSkill;
    this->_arms = nullptr;
}

MonsterCard::MonsterCard(const MonsterCard & other)
    : Card(other)
{
    this->_attack = other._attack;
    this->_hasSkill = other._hasSkill;
    this->_arms = nullptr;
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

void MonsterCard::removeArms()
{
    if (this->_arms == nullptr)
        return;
    this->_arms->_owner = nullptr;
    this->_arms = nullptr;
}

MonsterCard &MonsterCard::operator =(const MonsterCard & other)
{
    if(this == &other)
        return *this;
    Card::operator =(other);
    this->_attack = other._attack;
    this->_hasSkill = other._hasSkill;
    this->_arms = nullptr;
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
    return stream;
}

QDataStream &operator>>(QDataStream & stream, MonsterCard & card)
{
    stream >> (Card &) card;
    return stream;
}


