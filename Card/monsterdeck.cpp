#include "monsterdeck.h"

/**
 * @brief Monster_DragonBorn::MonsterCard 龙裔:五费六攻白板
 */
Monster_DragonBorn::Monster_DragonBorn()
{
    *_name = "龙裔";
    _attack = 6;
    _consume = 5;
}

int Monster_DragonBorn::getCategory()
{
    return Monster_DragonBorn::_category;
}

Monster_DogInScau::Monster_DogInScau()
{
    *_name = "华农的狗";
    _attack = 5;
    _consume = 4;
    //_hasSkill = true;
}

int Monster_DogInScau::skill()
{
    //TODO 华农的狗： 嘲讽

    return -1;
}

int Monster_DogInScau::getCategory()
{
    return Monster_DogInScau::_category;
}
