#include "magicdeck.h"
#include <QTcpSocket>

Magic_FireBall::Magic_FireBall()
{
    *_name = "火球术";
    *_description = "消耗4法力值，对玩家造成6点伤害";
    _consume = 4;
}

int Magic_FireBall::skill()
{
    //火球术: 利用socket向服务端发送对玩家/怪造成6伤害的报文
//    QTcpSocket * socket = dynamic_cast<QTcpSocket *> (this->_Tag);
//    QDataStream out(socket);
//    out.setVersion(QDataStream::Qt_5_9);
//    out<<1;
//    out<<this->_id;
//    out<<this->_target_id;
//    socket->flush();
}

int Magic_FireBall::getCategory() const
{
    return Magic_FireBall::_category;
}

void Magic_FireBall::setTarget_id(int target_id)
{
    _target_id = target_id;
}

Magic_WakeUp::Magic_WakeUp()
{
    *_name = "苏醒";
    *_description = "仅在本回合，获得额外的两点法力值";
    _consume = 0 ;
}

int Magic_WakeUp::skill()
{
    //苏醒：法力值加2
//    QTcpSocket * socket = dynamic_cast<QTcpSocket *> (this->_Tag);
//    QDataStream out(socket);
//    out.setVersion(QDataStream::Qt_5_9);
//    out<<3;
//    out<<2;
//    socket->flush();
}

int Magic_WakeUp::getCategory() const
{
    return Magic_WakeUp::_category;
}

Magic_KingSpell::Magic_KingSpell()
{
    *_name = "王者咏唱";
    *_description = "为你的一只怪兽增加4点攻击力";
    _consume = 4;
}

int Magic_KingSpell::skill()
{
    //王者咏唱: 己方场上的怪兽高亮化，无效其余点击，点击后为相应的怪兽添加攻击力
//    QTcpSocket * socket = dynamic_cast<QTcpSocket *> (this->_Tag);
//    QDataStream out(socket);
//    out.setVersion(QDataStream::Qt_5_9);
//    out<<1;
//    out<<this->_id;
//    out<<this->_target_id;
//    socket->flush();
}

int Magic_KingSpell::getCategory() const
{
    return Magic_KingSpell::_category;
}
