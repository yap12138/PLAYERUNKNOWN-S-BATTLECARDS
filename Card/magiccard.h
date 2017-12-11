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

    inline int getSkillBuff() const {  return this->_skillBuff; }

    CARDSHARED_EXPORT friend QDataStream &operator<<(QDataStream &, const MagicCard &);
    CARDSHARED_EXPORT friend QDataStream &operator>>(QDataStream &, MagicCard &);
protected:
    int _skillBuff;     //目标id
    static const int _category = 2;  //卡牌种类
};

#endif // MAGICCARD_H
