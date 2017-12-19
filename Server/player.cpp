#include "player.h"
#include "bootstrap.h"
#include <QDebug>


Player::Player(QTcpSocket * socket, QObject *parent)
    : QObject(parent), _clientSocket(*socket)
{
    this->_HP = 30;
    this->_consume = 0;
    initConsumQueue();
}
//172.16.31.9
void Player::initTotalCard(QStandardItemModel *model)
{
    //先将list里的卡牌清空
    if (!this->_totalCardDeck.isEmpty())
    {
        foreach (auto var, this->_totalCardDeck) {
            this->_totalCardDeck.remove(var->getId());
            qDebug()<<var->getCategory();
            qDebug()<<typeid(*var).name();
            delete var;
        }
    }
    //每一种类卡牌各两张放入卡组
    for (int i = 0; i < 2; i++)
    {
//        Card * var1 = new Magic_AGiftFromTeacher();
//        Card * var2 = new Magic_FireBall();
//        Card * var3 = new Magic_KingSpell();
//        Card * var4 = new Magic_WakeUp();
//        Card * var5 = new Magic_WindStrom();

        Card * var6 = new Monster_CrazyCow();
        Card * var7 = new Monster_Devil();
        Card * var8 = new Monster_DogInScau();
        Card * var9 = new Monster_DragonBorn();
        Card * var10 = new Monster_Peashooter();
        Card * var11 = new Monster_WhiteFriend();

//        Card * var12 = new Arms_98K();
//        Card * var13 = new Arms_ArchangelSword();
//        Card * var14 = new Arms_DeathfireStaff();
//        Card * var15 = new Arms_InfinityEdge();
//        Card * var16 = new Arms_Nokia();

//        this->_totalCardDeck.insert(var1->getId(), var1);
//        this->_totalCardDeck.insert(var2->getId(), var2);
//        this->_totalCardDeck.insert(var3->getId(), var3);
//        this->_totalCardDeck.insert(var4->getId(), var4);
//        this->_totalCardDeck.insert(var5->getId(), var5);
        this->_totalCardDeck.insert(var6->getId(), var6);
        this->_totalCardDeck.insert(var7->getId(), var7);
        this->_totalCardDeck.insert(var8->getId(), var8);
        this->_totalCardDeck.insert(var9->getId(), var9);
        this->_totalCardDeck.insert(var10->getId(), var10);
        this->_totalCardDeck.insert(var11->getId(), var11);
//        this->_totalCardDeck.insert(var12->getId(), var12);
//        this->_totalCardDeck.insert(var13->getId(), var13);
//        this->_totalCardDeck.insert(var14->getId(), var14);
//        this->_totalCardDeck.insert(var15->getId(), var15);
//        this->_totalCardDeck.insert(var16->getId(), var16);
    }

    this->_restCard = _totalCardDeck.values();

    int baseIndex = model->rowCount();
    for(int i = 0; i < _restCard.size(); i++)
    {
        model->setItem(baseIndex + i, 0, new QStandardItem(QString::number(this->_clientSocket.peerPort()) +" "+ this->_playerName));
        //设置字符颜色
        model->item(baseIndex + i,0)->setForeground(QBrush(QColor(255, 0, 0)));
        //设置字符位置
        model->item(baseIndex + i,0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(baseIndex + i, 1, new QStandardItem(QString::number(_restCard[i]->getId())));
        model->setItem(baseIndex + i, 2, new QStandardItem(_restCard[i]->getName()));
    }
}

int Player::getNextConsume()
{
    if (this->_consumeForTurn.size() <= 1)
        this->_consumeForTurn.enqueue(5);
    return this->_consumeForTurn.dequeue();
}

const Card *Player::getCardFromDeck()
{
    int index = qrand()%this->_restCard.size();
    Card * card = this->_restCard.takeAt(index);
    return card;
}


void Player::initConsumQueue()
{
    for(int i = 0; i < 10; i++)
    {
        this->_consumeForTurn.enqueue(10);
    }
}
