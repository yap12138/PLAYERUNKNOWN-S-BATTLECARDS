#include "magiccard.h"

MagicCard::MagicCard()
    : Card()
{

}

MagicCard::MagicCard(QString &name, QString &description, const int consume)
    : Card(name, description, consume)
{

}

MagicCard::MagicCard(const MagicCard & other)
    : Card(other)
{

}

int MagicCard::getCategory() const
{
    return this->_category;
}

QDataStream &operator<<(QDataStream & stream, const MagicCard & data)
{
    stream << (const Card &) data;
    return stream;
}

QDataStream &operator>>(QDataStream & stream, MagicCard & card)
{
    stream >> (Card &) card;
    return stream;
}