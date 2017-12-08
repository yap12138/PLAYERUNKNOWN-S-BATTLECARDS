#ifndef ARMSDECK_H
#define ARMSDECK_H
#include "armscard.h"

namespace PUBC {
    class Arms_98K;
}

class CARDSHARED_EXPORT Arms_98K : public ArmsCard
{
public:
    Arms_98K();

    virtual int skill();
    virtual int getCategory() const;

protected:
    static const int _category = 31;  //卡牌种类
};

class CARDSHARED_EXPORT Arms_InfinityEdge : public ArmsCard
{
public:
    Arms_InfinityEdge();

    virtual int skill();
    virtual int getCategory() const;

protected:
    static const int _category = 32;  //卡牌种类
};


#endif // ARMSDECK_H