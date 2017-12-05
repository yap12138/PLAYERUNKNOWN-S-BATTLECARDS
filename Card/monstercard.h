#ifndef MONSTERCARD_H
#define MONSTERCARD_H
#include "card.h"
#include "armscard.h"

namespace PUBC {
    class MonsterCard;
}

class CARDSHARED_EXPORT MonsterCard : public Card
{
public:
    friend class ArmsCard;

    MonsterCard();
    explicit MonsterCard(QString &name, QString &description, const int consume = 0, int attack = 0, bool hasSkill = false);
    MonsterCard(const MonsterCard&);
    //virtual ~MonsterCard();

    virtual int skill() = 0;    //卡牌效果
    virtual int getCategory() const;

    inline int getAttack() const { return this->_attack; }
    inline void setAttack(int newAttack) { this->_attack = newAttack; }
    inline bool hasSkill() const { return this->_hasSkill; }
    inline const ArmsCard * getArms() const { return this->_arms; }
    inline void setArms(ArmsCard & arms) { this->_arms = &arms; }

    MonsterCard & operator =(const MonsterCard &);

    void operator + (ArmsCard &);
    MonsterCard & operator - (MonsterCard &);
    friend QDataStream &operator<<(QDataStream &, const MonsterCard &);
    friend QDataStream &operator>>(QDataStream &, MonsterCard &);
protected:
    static const int _category = 1;  //卡牌种类

    int _attack;        //攻击值
    bool _hasSkill;     //含有技能？默认为false
    ArmsCard* _arms;    //持有的装备
};

#endif // MONSTERCARD_H