#include "magicdeck.h"

Magic_FireBall::Magic_FireBall()
{
    *_name = "火球术";
    *_description = "消耗4法力值，对玩家造成6点伤害";
    _consume = 4;
}

int Magic_FireBall::skill()
{
    //TODO 火球术: 利用socket向服务端发送对玩家造成6伤害的报文
}

int Magic_FireBall::getCategory() const
{
    return _category;
}

Magic_WakeUp::Magic_WakeUp()
{
    *_name = "苏醒";
    *_description = "仅在本回合，获得额外的两点法力值";
    _consume = 0 ;
}

int Magic_WakeUp::skill()
{
    //TODO 苏醒：法力值加2
}

int Magic_WakeUp::getCategory() const
{
    return _category;
}

Magic_KingSpell::Magic_KingSpell()
{
    *_name = "王者咏唱";
    *_description = "为你的一只怪兽增加4点攻击力";
    _consume = 4;
}

int Magic_KingSpell::skill()
{
    //TODO 王者咏唱: 己方场上的怪兽高亮化，无效其余点击，点击后为相应的怪兽添加攻击力
}

int Magic_KingSpell::getCategory() const
{
    return _category;
}
