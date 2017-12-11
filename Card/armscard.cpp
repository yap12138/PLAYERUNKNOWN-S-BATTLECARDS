#include "armscard.h"
#include "monstercard.h"

ArmsCard::ArmsCard()
    : Card()
{
    this->_attackBuff = 0;
    this->_owner = nullptr;
}

ArmsCard::ArmsCard(QString &name, QString &description, const int consume, int attackBuff)
    : Card(name, description, consume)
{
    this->_attackBuff = attackBuff;
    this->_owner = nullptr;
}

ArmsCard::ArmsCard(const ArmsCard & other)
    : Card(other)
{
    this->_attackBuff = other._attackBuff;
    this->_owner = nullptr;
}

int ArmsCard::skill()
{
    return -1;
}

int ArmsCard::getCategory() const
{
    return this->_category;
}

ArmsCard &ArmsCard::operator =(const ArmsCard &other)
{
    if(this == &other)
        return *this;
    Card::operator =(other);
    this->_attackBuff = other._attackBuff;
    this->_owner = nullptr;
    return *this;
}

ArmsCard &ArmsCard::operator -(MonsterCard & enemy)
{
    int a_value,b_value;
    if (enemy._arms != nullptr)
    {
        a_value = this->_attackBuff - enemy._arms->_attackBuff;
        b_value = enemy._arms->_attackBuff - this->_attackBuff;
        this->_attackBuff = a_value;
        enemy._arms->_attackBuff = b_value;
    }
    else
    {
        a_value = this->_attackBuff - enemy._attack;
        b_value = enemy._attack - this->_attackBuff;
        this->_attackBuff = a_value;
        enemy._attack = b_value;
    }
    return *this;
}

QDataStream &operator<<(QDataStream & stream, const ArmsCard & data)
{
    stream << (const Card &) data;
    return stream;
}

QDataStream &operator>>(QDataStream & stream, ArmsCard & card)
{
    stream >> (Card &) card;
    return stream;
}
