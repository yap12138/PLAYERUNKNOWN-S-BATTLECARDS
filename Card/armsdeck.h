#ifndef ARMSDECK_H
#define ARMSDECK_H
#include "armscard.h"

namespace PUBC {
    class Arms_98K;
    class Arms_InfinityEdge;
    class Arms_ArchangelSword;
    class Arms_DeathfireStaff;
}

/**
 * @brief The Arms_98K class Kar 98K
 */
class CARDSHARED_EXPORT Arms_98K : public ArmsCard
{
public:
    Arms_98K();

    virtual int skill();
    virtual int getCategory() const;

protected:
    static const int _category = 31;  //卡牌种类
};

/**
 * @brief The Arms_InfinityEdge class   无尽之刃
 */
class CARDSHARED_EXPORT Arms_InfinityEdge : public ArmsCard
{
public:
    Arms_InfinityEdge();

    virtual int skill();
    virtual int getCategory() const;

protected:
    static const int _category = 32;  //卡牌种类
};

/**
 * @brief The Arms_ArchangelSword class 大天使之剑
 */
class CARDSHARED_EXPORT Arms_ArchangelSword : public ArmsCard
{
public:
    Arms_ArchangelSword();

    virtual int skill();
    virtual int getCategory() const;

protected:
    static const int _category = 33;  //卡牌种类
};

/**
 * @brief The Arms_DeathfireStaff class 冥火法杖
 */
class CARDSHARED_EXPORT Arms_DeathfireStaff : public ArmsCard
{
public:
    Arms_DeathfireStaff();

    virtual int skill();
    virtual int getCategory() const;

protected:
    static const int _category = 34;  //卡牌种类
};


#endif // ARMSDECK_H