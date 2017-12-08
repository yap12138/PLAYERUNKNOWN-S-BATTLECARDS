#ifndef MONSTERDECK_H
#define MONSTERDECK_H
#include "monstercard.h"
#include "card.h"
namespace PUBC {
    class Monster_DragonBorn;
    class Monster_DogInScau;
    class Monster_CrazyCow;
    class Monster_Devil;
    class Monster_WhiteFriend;
    class Monster_Peashooter;
}

class CARDSHARED_EXPORT Monster_DragonBorn : public MonsterCard{
public :
    Monster_DragonBorn();
    virtual int getCategory() const;
private :
    static const int _category = 10;
};

class CARDSHARED_EXPORT Monster_DogInScau : public MonsterCard{
public :
    Monster_DogInScau();
    virtual int skill();
    virtual int getCategory() const;
private :
    static const int _category = 11;
};

class CARDSHARED_EXPORT Monster_CrazyCow : public MonsterCard{
public:
    Monster_CrazyCow();
    virtual int skill();
    virtual int getCategory() const;
private:
    static const int _category = 12;
};

class CARDSHARED_EXPORT Monster_Devil : public MonsterCard{
public:
    Monster_Devil();
    virtual int skill();
    virtual int getCategory() const;
private:
    static const int _category = 13;
};

class CARDSHARED_EXPORT Monster_WhiteFriend : public MonsterCard{
public:
    Monster_WhiteFriend();
    virtual int skill();
    virtual int getCategory() const;
private:
    static const int _category = 14;
};

class CARDSHARED_EXPORT Monster_Peashooter : public MonsterCard{
public:
    Monster_Peashooter();
    virtual int skill();
    virtual int getCategory() const;
private:
    static const int _category = 15;
};
#endif // MONSTERDECK_H
