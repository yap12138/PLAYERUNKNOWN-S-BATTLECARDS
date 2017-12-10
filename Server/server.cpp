#include "server.h"
#include <QTimer>

//qRegisterMetaType<Server>("Server");

int Server::_serialID = 0;

Server::Server(Player *p1, Player *p2, QObject *parent)
    : QObject(parent), _gamePair(p1,p2)
{
    this->_serverID = ++Server::_serialID;
    bindServer(p1->getSocket());
    bindServer(p2->getSocket());
    //给双方发送匹配成功
    sendMessage(p1, 0);
    sendMessage(p1, p2->getPlayerName());
    p1->getSocket().flush();
    sendMessage(p2, 0);
    sendMessage(p2, p1->getPlayerName());
    p2->getSocket().flush();
    //初始化双方卡组
    p1->initTotalCard();
    p2->initTotalCard();

    QTimer::singleShot(1000, this, SLOT(doGameStart()));
}

Server::~Server()
{

}

void Server::bindServer(QTcpSocket &socket)
{
    connect(&socket, SIGNAL(disconnected()), this, SLOT(doDisconnect()));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(doRequest()));
    connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(doError(QAbstractSocket::SocketError)));
}

/**
 * @brief Server::getPlayerFromSocket
 * @param socket
 * @param mode  0找自己，1找敌人
 * @return
 */
Player *Server::getPlayerFromSocket(QTcpSocket const * socket, int mode) const
{
    if ( (&(this->_gamePair.first->getSocket()) == socket && mode == 0)
         || (&(this->_gamePair.second->getSocket()) == socket && mode == 1) )
    {
        return this->_gamePair.first;
    }
    else
    {
        return this->_gamePair.second;
    }
}

void Server::sendMessage(Player * const player, int message)
{
    QTcpSocket* send = &(player->getSocket());
    QDataStream out(send);
    out.setVersion(QDataStream::Qt_5_9);
    out << message;
    qDebug()<<player->getPlayerName() << " out " << message << " succeed";
}

void Server::sendMessage(Player * const player, const QString &message)
{
    QTcpSocket* send = &(player->getSocket());
    QDataStream out(send);
    out.setVersion(QDataStream::Qt_5_9);
    out << message;
    qDebug()<<player->getPlayerName() << " out " << message << " succeed";
}

void Server::sendMessage(Player * const player, const Card *card)
{
    QTcpSocket* send = &(player->getSocket());
    QDataStream out(send);
    out.setVersion(QDataStream::Qt_5_9);
//    if (dynamic_cast<MagicCard*> (card))
//    {
//        MagicCard
//    }
    out<<(*card);
    qDebug()<<player->getPlayerName() << " out card:"<< card->getCategory() << " id:" << card->getId() << " succeed";
}
//172.16.31.9

void Server::doDisconnect()
{
    QTcpSocket* disSocket = static_cast<QTcpSocket*> (sender());
    Player * ePlayer = getPlayerFromSocket(disSocket, 1);
    qDebug()<<ePlayer->getSocket().peerPort();
    QTcpSocket * enemySocket = &(ePlayer->getSocket());
    //发送给对方，己方已经断开，游戏结束
    QDataStream out(enemySocket);
    out.setVersion(QDataStream::Qt_5_9);
    out<<8;
    enemySocket->flush();

    Player * mPlayer = getPlayerFromSocket(disSocket, 0);
    qDebug()<<mPlayer->getSocket().peerPort();
    mPlayer->deleteLater();
    emit resetPlayer(ePlayer, this);    //发送信号，将未掉线一方重新进入匹配队列
}

void Server::doRequest()
{
    qDebug()<<"new request";
    QTcpSocket* rev = static_cast<QTcpSocket*>(sender());
    QDataStream in(rev);
    in.setVersion(QDataStream::Qt_5_9);
    int msgCategory;
    in>>msgCategory;
    switch (msgCategory) {
    case 0:
        break;
    case 1:
        {
            int sourceID, targetID;
            in >> sourceID;
            in >> targetID;
            Player* sourcePlayer = this->getPlayerFromSocket(rev, 0);
            Player* targetPlayer = this->getPlayerFromSocket(rev, 1);
            Card* sourceCard = sourcePlayer->getCard(sourceID);
            if (sourcePlayer->isMyCard(targetID)) targetPlayer = sourcePlayer;  //判断是否对自己的怪物使用卡牌, 是则从自己卡组寻找卡牌
            if (targetID == -2) {

                //怪物上场，减少使用者法力值
                sourcePlayer->setConsume(sourcePlayer->getConsume() - sourceCard->getConsume());

                //告诉对手自己所上场的怪物
                sendMessage(targetPlayer, 3);
                sendMessage(targetPlayer, sourceCard->getCategory());
                sendMessage(targetPlayer, sourceCard->getId());
                sendMessage(targetPlayer, -2);
                targetPlayer->getSocket().flush();

                //告诉对手更新自己的血和法力值信息
                sendMessage(targetPlayer, 2);
                sendMessage(targetPlayer, 1);
                sendMessage(targetPlayer, sourcePlayer->getHP());
                sendMessage(targetPlayer, sourcePlayer->getConsume());
                targetPlayer->getSocket().flush();

                break;
            }
            Card* targetCard = targetPlayer->getCard(targetID);

            //出牌信息转发
            sendMessage(targetPlayer, 3);
            sendMessage(targetPlayer, sourceCard->getCategory());
            sendMessage(targetPlayer, sourceCard->getId());
            sendMessage(targetPlayer, targetCard->getId());
            targetPlayer->getSocket().flush();

            if (targetID == -1){
                monsterAttack(sourceCard, targetPlayer, sourcePlayer, targetPlayer); //怪物对人
            }
            else if (sourceID < 20){
                monsterAttack(sourceCard, targetCard, sourcePlayer, targetPlayer); //怪物对怪物
            }
            else if (sourceID < 30){
                //TODO 魔法对怪
            }
            else if (sourceID < 40){
                sourcePlayer->setConsume(sourcePlayer->getConsume() - sourceCard->getConsume());    //出装备卡扣费

                //告诉对手更新自己的信息
                sendMessage(targetPlayer, 2);
                sendMessage(targetPlayer, 1);
                sendMessage(targetPlayer, sourcePlayer->getHP());
                sendMessage(targetPlayer, sourcePlayer->getConsume());
                targetPlayer->getSocket().flush();

                addArms(sourceCard, targetCard, sourcePlayer, targetPlayer);    //怪物装备武器
            }
            break;
        }
    default:
        break;
    }
}

//怪物对怪物攻击
void Server::monsterAttack(Card* source, Card* target, Player* sourcePlayer, Player* targetPlayer){
    MonsterCard* tempS = dynamic_cast<MonsterCard* >(source);
    MonsterCard* tempT = dynamic_cast<MonsterCard* >(target);
    bool SHaveArms = (tempS->getArms() != nullptr);
    bool THaveArms = (tempT->getArms() != nullptr);
    *tempS - *tempT;

    if (SHaveArms){
        sendMessage(sourcePlayer, 4);
        sendMessage(sourcePlayer, source->getId());
        sendMessage(sourcePlayer, tempS->getArms()->getAttackBuf());
    }
    else {
        sendMessage(sourcePlayer, 5);
        sendMessage(sourcePlayer, source->getId());
        sendMessage(sourcePlayer, tempS->getAttack());
    }
    sourcePlayer->getSocket().flush();

    if (THaveArms){
        sendMessage(targetPlayer, 4);
        sendMessage(targetPlayer, source->getId());
        sendMessage(targetPlayer, tempT->getArms()->getAttackBuf());
    }
    else {
        sendMessage(targetPlayer, 5);
        sendMessage(targetPlayer, source->getId());
        sendMessage(targetPlayer, tempT->getAttack());
    }
    targerPlayer->getSocket().flush();
}

//怪物对人攻击
void Server::monsterAttack(Card* source, Player* target, Player* sourcePlayer, Player* targetPlayer){
    target->setHP(target->getHP() - dynamic_cast<MonsterCard* >(source)->getAttack());
    sendMessage(sourcePlayer, 2);
    sendMessage(sourcePlayer, 1);
    sendMessage(sourcePlayer, targetPlayer->getHP());
    sendMessage(sourcePlayer, targetPlayer->getConsume());
    sourcePlayer->getSocket().flush();

    sendMessage(targetPlayer, 2);
    sendMessage(targetPlayer, 0);
    sendMessage(targetPlayer, targetPlayer->getHP());
    sendMessage(targetPlayer, targetPlayer->getConsume());
    targerPlayer->getSocket().flush();

    if (targetPlayer->getHP() <= 0)
    {
        sendMessage(sourcePlayer, 10);
        sendMessage(sourcePlayer, 1);
        sourcePlayer->getSocket().flush();

        sendMessage(targetPlayer, 10);
        sendMessage(targetPlayer, 0);
        targetPlayer->getSocket().flush();

        delete sourcePlayer();
        delete targetPlayer();
        delete this;
    }
}

//怪物装备武器
void Server::addArms(Card *sourceArms, Card *targetMonster, Player *sourcePlayer, Player *targetPlayer)
{
    dynamic_cast<MonsterCard*>(targetMonster) + dynamic_cast<ArmsCard*>(sourceArms);
    sendMessage(sourcePlayer, 4);
    sendMessage(sourcePlayer, sourceArms->getId());
    sendMessage(sourcePlayer, tempS->getArms()->getAttackBuf());
    sourcePlayer->getSocket().flush();

    sendMessage(targetPlayer, 4);
    sendMessage(targetPlayer, sourceArms->getId());
    sendMessage(targetPlayer, tempS->getArms()->getAttackBuf());
    targerPlayer->getSocket().flush();
}

void Server::doError(QAbstractSocket::SocketError e)
{
    qDebug()<< e;
}

/**
 *  给双方发4张初始手牌，再确定一个先攻，再发牌发费
 * @brief Server::doStartGame
 */
void Server::doGameStart()
{
    qDebug()<<"do game start";
    //0表示1p先攻，1表示2p先攻
    int whoToFirst = qrand()%2;
    Player * p1 = this->_gamePair.first;
    Player * p2 = this->_gamePair.second;
    //开局报头
    sendMessage(p1, 6);
    sendMessage(p2, 6);
    //4张卡牌
    for (int i = 0; i < 4; i++)
    {
        sendMessage(p1, p1->getCardFromDeck());
        sendMessage(p2, p2->getCardFromDeck());
    }
    p1->getSocket().flush();
    p2->getSocket().flush();

    this->_nextTurn = whoToFirst;
    QTimer::singleShot(1000, this, SLOT(doTurnStart()));
}

void Server::doTurnStart()
{
    qDebug()<<this->_nextTurn;
    Player * player = (this->_nextTurn == 0)? this->_gamePair.first : this->_gamePair.second;
    qDebug()<<player->getSocket().peerPort();

    sendMessage(player, 1);
    sendMessage(player, player->getNextConsume());
    sendMessage(player, player->getCardFromDeck());
    player->getSocket().flush();
}


