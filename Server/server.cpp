#include "server.h"

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
}

void Server::bindServer(QTcpSocket & socket)
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
Player *Server::getPlayerFromSocket(const QTcpSocket * socket, int mode) const
{
    if ( &(this->_gamePair.first->getSocket()) == socket && mode == 0 )
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
    QTcpSocket* send = static_cast<QTcpSocket*>(&player->getSocket());
    QDataStream out(send);
    out.setVersion(QDataStream::Qt_5_9);
    out << message;
    qDebug()<<player->getPlayerName() + " out " + message + " succeed";
}

void Server::sendMessage(Player * const player, QString message)
{
    QTcpSocket* send = static_cast<QTcpSocket*>(&player->getSocket());
    QDataStream out(send);
    out.setVersion(QDataStream::Qt_5_9);
    out << message;
    qDebug()<<player->getPlayerName() + " out " + message + " succeed";
}


void Server::doDisconnect()
{
    QTcpSocket* disSocket = qobject_cast<QTcpSocket*> (sender());

    Player * ePlayer = getPlayerFromSocket(disSocket, 1);
    QTcpSocket * enemySocket = &(ePlayer->getSocket());
    //发送给对方，己方已经断开，游戏结束
    QDataStream out(enemySocket);
    out.setVersion(QDataStream::Qt_5_9);
    out<<8;
    enemySocket->flush();

    getPlayerFromSocket(disSocket, 0)->deleteLater();
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

