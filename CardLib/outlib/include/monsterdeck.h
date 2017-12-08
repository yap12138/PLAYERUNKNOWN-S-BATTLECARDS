#ifndef MONSTERDECK_H
#define MONSTERDECK_H
#include "monstercard.h"
#include "card.h"
namespace PUBC {
class Monster_DragonBorn;
class Monster_DogInScau;
}

class CARDSHARED_EXPORT Monster_DragonBorn : public MonsterCard{
public :
    Monster_DragonBorn();
    virtual int getCategory();
private :
    static const int _category = 10;
};

class CARDSHARED_EXPORT Monster_DogInScau : public MonsterCard{
public :
    Monster_DogInScau();
    virtual int skill();
    virtual int getCategory();
private :
    static const int _category = 11;
};
#endif // MONSTERDECK_H
