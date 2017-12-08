#include "monsterdeck.h"

/**
 * @brief Monster_DragonBorn::MonsterCard 龙裔:五费六攻白板
 */
Monster_DragonBorn::Monster_DragonBorn()
{
    *this->_name = "龙裔";
    *this->_description = "来自上古卷轴的龙之后裔，总是大吼着fosroda的危险人物";
    _attack = 6;
    _consume = 5;
}

int Monster_DragonBorn::getCategory() const
{
    return Monster_DragonBorn::_category;
}

Monster_DogInScau::Monster_DogInScau()
{
    *this->_name = "华农的狗";
    *this->_description = "这是一条来自华农的狗  (ΦωΦ)  总喜欢狂吠";
    _attack = 3;
    _consume = 3;
    _hasSkill = true;
}

int Monster_DogInScau::skill()
{
    //TODO 华农的狗： 嘲讽

    return 1;
}

int Monster_DogInScau::getCategory() const
{
    return Monster_DogInScau::_category;
}

Monster_CrazyCow::Monster_CrazyCow()
{
    *_name = "狂牛";
    *_description = "发了疯的野牛！(▼皿▼#)";
    _attack = 5;
    _consume = 6;
    _hasSkill = true;
}

/**
 * @brief Monster_CrazyCow::skill   随机对一个怪造成伤害，5伤害
 * @return
 */
int Monster_CrazyCow::skill()
{
    //TODO 冲锋
    return 1;
}

int Monster_CrazyCow::getCategory() const
{
    return Monster_CrazyCow::_category;
}

Monster_Devil::Monster_Devil()
{
    *this->_name = "远古恶魔";
    *this->_description = "来自东方远古的邪恶力量，令人窒息的操作让对手瑟瑟发抖";
    this->_attack = 10;
    this->_consume = 9;
}

int Monster_Devil::skill()
{
    return -1;
}

int Monster_Devil::getCategory() const
{
    return Monster_Devil::_category;
}

Monster_WhiteFriend::Monster_WhiteFriend()
{
    *this->_name = "五五开的好友";
    *this->_description = "人类臆想的生物, 曾为某电竞主播挡刀";
    this->_attack = 2;
    this->_consume = 3;
    _hasSkill = true;
}

int Monster_WhiteFriend::skill()
{
    //TODO 五五开的好友: 嘲讽， 随机对一个怪造成3伤害。
    return 1;
}

int Monster_WhiteFriend::getCategory() const
{
    return Monster_WhiteFriend::_category;
}


Monster_Peashooter::Monster_Peashooter()
{
    *this->_name = "豌豆射手";
    *this->_description = "豌豆射手可谓你的第一道防线，他们朝来犯的敌人射击豌豆。";
    this->_attack = 2;
    this->_consume = 2;
    _hasSkill = false;
}

int Monster_Peashooter::skill()
{
    return -1;
}

int Monster_Peashooter::getCategory() const
{
    return Monster_Peashooter::_category;
}
