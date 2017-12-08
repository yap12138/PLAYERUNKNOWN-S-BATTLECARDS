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

    void setTarget_id(int target_id);

private:
    int _target_id;

protected:
    static const int _category = 20;
};

class CARDSHARED_EXPORT Magic_WakeUp: public MagicCard {
public:
    Magic_WakeUp();
    virtual int skill();
    virtual int getCategory() const;
protected:
    static const int _category = 21;
};

class CARDSHARED_EXPORT Magic_KingSpell: public MagicCard{
public:
    Magic_KingSpell();
    virtual int skill();
    virtual int getCategory() const;
private:
    int _target_id;
protected:
    static const int _category = 22;
};

#endif // MAGICDECK_H
