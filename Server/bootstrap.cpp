#include "bootstrap.h"
#include "ui_widget.h"
#include <QThread>


ServerBootstrap::ServerBootstrap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initServer();
    initConnect();
}

ServerBootstrap::~ServerBootstrap()
{
    delete ui;
    delete _server;
}

void ServerBootstrap::initServer()
{
    _server = new QTcpServer(this);
    //_localHost = getHostConnectedIP();
    _localHost = getHostConnectedIP();
    _server->listen(_localHost, 5000);
    connect(_server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    this->ui->lab_server->setText(_localHost.toString());
}

void ServerBootstrap::initConnect()
{
    connect(this, SIGNAL(canMatch()), this, SLOT(doMatch()), Qt::QueuedConnection);
}

/**
 * @brief Widget::getHostConnectedIP 获取本地可用ip
 * @return
 */
QHostAddress ServerBootstrap::getHostConnectedIP() const
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, list) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp)
                && interface.flags().testFlag(QNetworkInterface::IsRunning)
                && !interface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            QList<QNetworkAddressEntry> qlist = interface.addressEntries();
            //qDebug()<<interface.humanReadableName();
            if (interface.humanReadableName().contains("VirtualBox") || interface.humanReadableName().contains("本地连接"))
                continue;
            foreach (QNetworkAddressEntry address, qlist) {
                if (address.ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    //qDebug()<<address.ip().toString();
                    return address.ip();
                }
            }
        }
    }
    throw QString("ip not found");
}

/**
 * @brief Widget::getPlayerFromSocket 用socket找出对应的player
 * @param socket
 * @return
 */
Player *ServerBootstrap::getPlayerFromSocket(QTcpSocket const * socket) const
{
    //在匹配队列找
    foreach (auto var, this->_playerQueue) {
        if ( &(var->getSocket()) == socket )
            return var;
    }
    return nullptr;
}

void ServerBootstrap::getClientInfo(QTcpSocket * const socket, QDataStream &stream)
{
    QString name;
    stream>>name;
    qDebug()<<name;
    this->ui->textBrowser->append(name);
    Player * var = getPlayerFromSocket(socket);
    var->setPlayerName(name);

    if (_playerQueue.size() >= 2)
        emit canMatch();
}

void ServerBootstrap::acceptConnection()
{
    QTcpSocket* clientConnection = _server->nextPendingConnection();
    qDebug()<<"new connect"<<clientConnection->peerAddress().toString()<<" : "<<clientConnection->peerPort();
    this->ui->textBrowser->append("new connect"+clientConnection->peerAddress().toString()+" : "+QString::number(clientConnection->peerPort()));
    connect(clientConnection, SIGNAL(disconnected()), this, SLOT(onDisConnect()));
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(doRequest()));
    connect(clientConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(showError(QAbstractSocket::SocketError)));
    //玩家连接，进入队列等待
    Player* player = new Player(clientConnection, this);
    this->_playerQueue.enqueue(player);

    qDebug()<<"queue new size:"<<this->_playerQueue.size();
    this->ui->textBrowser->append("queue new size: "+QString::number(this->_playerQueue.size()));
}

void ServerBootstrap::onDisConnect()
{
    QTcpSocket* disSocket = qobject_cast<QTcpSocket*> (sender());

    Player * tPlayer = getPlayerFromSocket( disSocket );
    this->_playerQueue.removeOne(tPlayer);

    qDebug()<<"queue new size:"<<this->_playerQueue.size();
    disSocket->deleteLater();
}

void ServerBootstrap::doRequest()
{
    qDebug()<<"new request";
    QTcpSocket* rev = static_cast<QTcpSocket*>(sender());
    QDataStream in(rev);
    in.setVersion(QDataStream::Qt_5_9);
    int msgCategory;
    in>>msgCategory;
    switch (msgCategory) {
    case 0:
        getClientInfo(rev, in);
        break;
    default:
        break;
    }
}

void ServerBootstrap::doMatch()
{
    Player* p1 = this->_playerQueue.dequeue();
    Player* p2 = this->_playerQueue.dequeue();
    //转移给服务线程处理
    /*p1->getSocket().disconnect(this);
    p2->getSocket().disconnect(this);*/
    disconnect(&(p1->getSocket()), SIGNAL(disconnected()), this, SLOT(onDisConnect()));
    disconnect(&(p1->getSocket()), SIGNAL(readyRead()), this, SLOT(doRequest()));
    disconnect(&(p1->getSocket()), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(showError(QAbstractSocket::SocketError)));
    disconnect(&(p2->getSocket()), SIGNAL(disconnected()), this, SLOT(onDisConnect()));
    disconnect(&(p2->getSocket()), SIGNAL(readyRead()), this, SLOT(doRequest()));
    disconnect(&(p2->getSocket()), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(showError(QAbstractSocket::SocketError)));

    Server* server = new Server(p1, p2);
    //QThread * serverThread = new QThread(server);
    //server->moveToThread(serverThread);

    connect(server, SIGNAL(resetPlayer(Player*,Server*)), this, SLOT(doReset(Player*,Server*)));
    connect(server, SIGNAL(gameOver(Server*)), this, SLOT(doGameover(Server*)));
    //保存对局信息
    this->_matchedList.insert(p1, server);
    this->_matchedList.insert(p2, server);

    qDebug()<<"match succeed\n"<<"have "<<this->_matchedList.size()/2<<" match pair";
    this->ui->textBrowser->append("match succeed\nhave "+QString::number(this->_matchedList.size()/2)+" match pair");
}

void ServerBootstrap::showError(QAbstractSocket::SocketError e)
{
    qDebug()<< e;
    QTcpSocket * socket = static_cast<QTcpSocket*> (sender());
    this->ui->textBrowser->append(socket->peerAddress().toString() + " : " + QString::number(socket->peerPort()) + " error");
}

void ServerBootstrap::doReset(Player * player, Server * server)
{
    qDebug()<<"remove "<<player->getPlayerName()<<" to queue";
    this->_playerQueue.enqueue(player);        //将对手放回就绪匹配队列
    connect(&(player->getSocket()), SIGNAL(disconnected()), this, SLOT(onDisConnect()));
    connect(&(player->getSocket()), SIGNAL(readyRead()), this, SLOT(doRequest()));
    connect(&(player->getSocket()), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(showError(QAbstractSocket::SocketError)));
    this->_matchedList.remove(server->getPlayers().first);
    this->_matchedList.remove(server->getPlayers().second);
    server->deleteLater();
}

void ServerBootstrap::doGameover(Server * server)
{
    qDebug()<<"game over";
    this->ui->textBrowser->append("game over\n");
    this->_matchedList.remove(server->getPlayers().first);
    this->_matchedList.remove(server->getPlayers().second);
    server->getPlayers().first->getSocket().disconnectFromHost();
    server->getPlayers().second->getSocket().disconnectFromHost();
    //回收内存
    server->getPlayers().first->deleteLater();
    server->getPlayers().second->deleteLater();
    server->deleteLater();
}
