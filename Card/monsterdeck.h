#ifndef MONSTERDECK_H
#define MONSTERDECK_H
#include "monstercard.h"
#include "card.h"
namespace PUBC {

}

class Monster_DragonBorn : public MonsterCard{
public :
    Monster_DragonBorn();
};

class Monster_DogInScau : public MonsterCard{
public :
    Monster_DogInScau();

    virtual int skill();
}
#endif // MONSTERDECK_H
