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

    virtual int skill();    //一般返回卡牌攻击，特殊则处理攻击里加持值
    virtual int getCategory() const;

    inline int getAttackBuf() const { return this->_attackBuff; }
    inline void setAttackBuf(int newAttackBuf) { this->_attackBuff = newAttackBuf; }
    inline const MonsterCard & getOwner() const { return *this->_owner; }
    inline void setOwner(MonsterCard & owner) { this->_owner = &owner; }

    ArmsCard & operator = (const ArmsCard & other);
    ArmsCard & operator - (MonsterCard &);
    CARDSHARED_EXPORT friend QDataStream &operator<<(QDataStream &, const ArmsCard &);
    CARDSHARED_EXPORT friend QDataStream &operator>>(QDataStream &, ArmsCard &);
protected:
    static const int _category = 3;  //卡牌种类

    int _attackBuff;    //攻击力加成
    MonsterCard* _owner;    //武器卡拥有者
};

#endif // ARMS_H
