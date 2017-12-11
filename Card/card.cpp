#include "card.h"

int Card::_count = 0;

Card::Card()
{
    this->_id = _count++;
    this->_name = new QString();
    this->_description = new QString();
}

Card::Card(QString &name, QString &description, const int consume)
{
    this->_id = _count++;
    this->_name = new QString();
    this->_description = new QString();
    *this->_name = name;
    *this->_description = description;
    this->_consume = consume;
}

Card::Card(const Card & other)
{
    this->_id = _count++;
    this->_name = new QString();
    this->_description = new QString();
    this->_name = other._name;
    this->_description = other._description;
    this->_consume = other._consume;
}

Card::~Card()
{
}

int Card::skill()
{
    return -1;
}

int Card::getCategory() const
{
    return this->_category;
}

Card & Card::operator =(const Card & other)
{
    if(this == &other)
        return *this;
    this->_id = _count++;
    //this->_category = other._category;
    this->_name = other._name;
    this->_description = other._description;
    this->_consume = other._consume;
    return *this;
}

QDataStream &operator<<(QDataStream & outStream, const Card & dataCard)
{
    if (outStream.version()!=QDataStream::Qt_5_9)
        outStream.setVersion(QDataStream::Qt_5_9);
    outStream << dataCard.getCategory();  //输出卡片类别信息，给服务端做处理
    outStream << dataCard._id;
    return outStream;
}

QDataStream &operator>>(QDataStream &inStream, Card & dataCard)
{
    if (inStream.version()!=QDataStream::Qt_5_9)
        inStream.setVersion(QDataStream::Qt_5_9);
    inStream >> dataCard._id;
    return inStream;
}





