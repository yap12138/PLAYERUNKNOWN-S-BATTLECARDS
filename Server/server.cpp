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
    qDebug()<<player->getPlayerName() + " out " + message + " succeed";
}

void Server::sendMessage(Player * const player, const QString &message)
{
    QTcpSocket* send = &(player->getSocket());
    QDataStream out(send);
    out.setVersion(QDataStream::Qt_5_9);
    out << message;
    qDebug()<<player->getPlayerName() + " out " + message + " succeed";
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
    qDebug()<<player->getPlayerName() + " out card:" + card->getCategory() + " id:" + card->getId() + " succeed";
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
    default:
        break;
    }
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

    if(whoToFirst == 0)
    {
        QTimer::singleShot(1000, this, SLOT(doTurnStart(p1)));
    }
    else
    {
        QTimer::singleShot(1000, this, SLOT(doTurnStart(p2)));
    }
}

void Server::doTurnStart(Player * const player)
{
    sendMessage(player, 2);
    sendMessage(player, player->getNextConsume());
    sendMessage(player, player->getCardFromDeck());
}


