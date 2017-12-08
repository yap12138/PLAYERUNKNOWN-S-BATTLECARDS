#ifndef ARMS_H
#define ARMS_H
#include "card.h"

class MonsterCard;

namespace PUBC {
    class ArmsCard;
}

class CARDSHARED_EXPORT ArmsCard : public Card
{
public:
    friend class MonsterCard;

    ArmsCard();
    explicit ArmsCard(QString &name, QString &description, const int consume = 0, int attackBuff = 0);
    ArmsCard(const ArmsCard &);

    virtual int skill();
    virtual int getCategory() const;

    inline int getAttackBuf() const { return this->_attackBuff; }
    inline void setAttackBuf(int newAttackBuf) { this->_attackBuff = newAttackBuf; }
    inline const MonsterCard & getOwner() const { return *this->_owner; }
    inline void setOwner(MonsterCard & owner) { this->_owner = &owner; }

    ArmsCard & operator = (const ArmsCard & other);
    ArmsCard & operator - (MonsterCard &);
    friend QDataStream &operator<<(QDataStream &, const ArmsCard &);
    friend QDataStream &operator>>(QDataStream &, ArmsCard &);
protected:
    static const int _category = 3;  //卡牌种类

    int _attackBuff;    //攻击力加成
    MonsterCard* _owner;    //武器卡拥有者
};

#endif // ARMS_H
