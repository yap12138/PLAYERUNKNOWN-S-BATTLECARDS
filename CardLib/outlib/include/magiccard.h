#ifndef MAGICCARD_H
#define MAGICCARD_H
#include "card.h"

namespace PUBC {
    class MagicCard;
}

class CARDSHARED_EXPORT MagicCard : public Card
{
public:
    MagicCard();
    explicit MagicCard(QString &name, QString &description, const int consume = 0);
    MagicCard(const MagicCard &);

    virtual int skill();    //卡牌效果
    virtual int getCategory() const;

    friend QDataStream &operator<<(QDataStream &, const MagicCard &);
    friend QDataStream &operator>>(QDataStream &, MagicCard &);
protected:
    static const int _category = 2;  //卡牌种类
};

#endif // MAGICCARD_H
