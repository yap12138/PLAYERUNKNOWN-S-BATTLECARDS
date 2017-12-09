#include "player.h"
#include "bootstrap.h"

Player::Player(QTcpSocket * socket, QObject *parent)
    : QObject(parent), _clientSocket(*socket)
{
    initConsumQueue();

}

void Player::initTotalCard()
{
    //先将list里的卡牌清空
    if (!this->_totalCardDeck.isEmpty())
    {
        foreach (auto var, this->_totalCardDeck) {
            delete var;
        }
    }
    //每一种类卡牌各两张放入卡组
    for (int i = 0; i < 2; i++)
    {
        Card * var1 = new Magic_AGiftFromTeacher();
        Card * var2 = new Magic_FireBall();
        Card * var3 = new Magic_KingSpell();
        Card * var4 = new Magic_WakeUp();
        Card * var5 = new Magic_WindStrom();

        Card * var6 = new Monster_CrazyCow();
        Card * var7 = new Monster_Devil();
        Card * var8 = new Monster_DogInScau();
        Card * var9 = new Monster_DragonBorn();
        Card * var10 = new Monster_Peashooter();
        Card * var11 = new Monster_WhiteFriend();

        Card * var12 = new Arms_98K();
        Card * var13 = new Arms_ArchangelSword();
        Card * var14 = new Arms_DeathfireStaff();
        Card * var15 = new Arms_InfinityEdge();
        Card * var16 = new Arms_Nokia();

        this->_totalCardDeck.append(var1);
        this->_totalCardDeck.append(var2);
        this->_totalCardDeck.append(var3);
        this->_totalCardDeck.append(var4);
        this->_totalCardDeck.append(var5);
        this->_totalCardDeck.append(var6);
        this->_totalCardDeck.append(var7);
        this->_totalCardDeck.append(var8);
        this->_totalCardDeck.append(var9);
        this->_totalCardDeck.append(var10);
        this->_totalCardDeck.append(var11);
        this->_totalCardDeck.append(var12);
        this->_totalCardDeck.append(var13);
        this->_totalCardDeck.append(var14);
        this->_totalCardDeck.append(var15);
        this->_totalCardDeck.append(var16);
    }
}

void Player::initConsumQueue()
{
    for(int i = 0; i < 10; i++)
    {
        this->_consumeForTurn.enqueue(3);
    }
}
