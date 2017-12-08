#include "armsdeck.h"
#include "monstercard.h"

Arms_98K::Arms_98K()
{
    *_name = "Kar 98K";
    *_description = "Kar.98k是二战期间产量最多的轻武器之一，被认为是第二次世界大战中最好的旋转后拉式枪机步枪之一。";
    this->_consume = 5;
    this->_attackBuff = 6;
}

int Arms_98K::skill()
{
    return this->_attackBuff;
}

int Arms_98K::getCategory() const
{
    return Arms_98K::_category;
}

Arms_InfinityEdge::Arms_InfinityEdge()
{
    *_name = "无尽之刃";
    *_description = "拥有恐怖的暴击能力，给伤害是装备怪兽的2倍";
    this->_consume = 6;
    this->_attackBuff = -999;
}

int Arms_InfinityEdge::skill()
{
    this->_attackBuff = this->_owner->getAttack()*2;
    return this->_attackBuff;
}

int Arms_InfinityEdge::getCategory() const
{
    return Arms_InfinityEdge::_category;
}

Arms_ArchangelSword::Arms_ArchangelSword()
{
    *_name = "大天使之剑";
    *_description = "是兄弟就来砍我,我在PLAYERUNKNOWN'S BATTLECARDS里等你";
    this->_consume = 2;
    this->_attackBuff = 3;
}

int Arms_ArchangelSword::skill()
{
    return this->_attackBuff;
}

int Arms_ArchangelSword::getCategory() const
{
    return Arms_ArchangelSword::_category;
}

Arms_DeathfireStaff::Arms_DeathfireStaff()
{
    *_name = "冥火法杖";
    *_description = "冥火法杖来自地狱。也只有来自地狱的人才可能会拥有它。";
    this->_consume = 4;
    this->_attackBuff = 5;
}

int Arms_DeathfireStaff::skill()
{
    return this->_attackBuff;
}

int Arms_DeathfireStaff::getCategory() const
{
    return Arms_DeathfireStaff::_category;
}

Arms_Nokia::Arms_Nokia()
{
    *_name = "诺基亚";
    *_description = "充满悲情色彩的手机";
    this->_consume = 9;
    this->_attackBuff = 10;
}

int Arms_Nokia::skill()
{
    return this->_attackBuff;
}

int Arms_Nokia::getCategory() const
{
    return Arms_Nokia::_category;
}
