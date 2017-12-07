#ifndef MAGICDECK_H
#define MAGICDECK_H
#include "card.h"
#include "magiccard.h"


namespace  PUBC {
 class Magic_FireBall;
 class Magic_KingSpell;
 class Magic_WakeUp;
}

class CARDSHARED_EXPORT Magic_FireBall: public MagicCard{
public:
    Magic_FireBall();
    virtual int skill();
    virtual int getCategory() const;
private:
    static const int _category = 20;
};

class CARDSHARED_EXPORT Magic_WakeUp: public MagicCard {
public:
    Magic_WakeUp();
    virtual int skill();
    virtual int getCategory() const;

};

class CARDSHARED_EXPORT Magic_KingSpell: public MagicCard{
public:
    Magic_KingSpell();
    virtual int skill();
    virtual int getCategory() const;
private:
    static const int _category = 22;
};

#endif // MAGICDECK_H
