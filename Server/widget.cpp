#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initServer();
    initConnect();
}

Widget::~Widget()
{
    delete ui;
    delete _server;
}

void Widget::initServer()
{
    _server = new QTcpServer(this);
    //_localHost = getHostConnectedIP();
    _localHost = getHostConnectedIP();
    _server->listen(_localHost, 5000);
    connect(_server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    this->ui->lab_server->setText(_localHost.toString());
}

void Widget::initConnect()
{
    connect(this, SIGNAL(canMatch()), this, SLOT(doMatch()), Qt::QueuedConnection);
}

/**
 * @brief Widget::getHostConnectedIP 获取本地可用ip
 * @return
 */
QHostAddress Widget::getHostConnectedIP() const
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
Player *Widget::getPlayerFromSocket(QTcpSocket const * socket) const
{
    //先在匹配队列找
    foreach (auto var, this->_playerQueue) {
        if ( &(var->getSocket()) == socket )
            return var;
    }
    //匹配对局里找
    QTcpSocket* tSocket = const_cast<QTcpSocket *>(socket);
    PlayerPair temp = this->_matchedList[tSocket];
    if ( &(temp.first->getSocket()) == socket )
    {
        return temp.first;
    }
    else
    {
        return temp.second;
    }
}

Player *Widget::getEnemyFromSocket(const QTcpSocket * socket) const
{
    QTcpSocket* tSocket = const_cast<QTcpSocket *>(socket);
    PlayerPair temp = this->_matchedList[tSocket];
    if ( &(temp.first->getSocket()) == socket )
    {
        return temp.second;
    }
    else
    {
        return temp.first;
    }
}

void Widget::getClientInfo(QTcpSocket * const socket, QDataStream &stream)
{
    QString name;
    stream>>name;
    qDebug()<<name;
    Player * var = getPlayerFromSocket(socket);
    var->setPlayerName(name);

    if (_playerQueue.size() >= 2)
        emit canMatch();
}

void Widget::acceptConnection()
{
    QTcpSocket* clientConnection = _server->nextPendingConnection();
    qDebug()<<"new connect"<<clientConnection->localAddress().toString();
    connect(clientConnection, SIGNAL(disconnected()), this, SLOT(onDisConnect()));
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(doRequest()));
    connect(clientConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(showError(QAbstractSocket::SocketError)));
    //玩家连接，进入队列等待
    Player* player = new Player(clientConnection, this);
    this->_playerQueue.enqueue(player);

    qDebug()<<"queue new size:"<<this->_playerQueue.size();

}

void Widget::onDisConnect()
{
    QTcpSocket* disSocket = qobject_cast<QTcpSocket*> (sender());

//    QProcess* cmd = new QProcess;
//    qDebug() << disSocket->peerAddress().toString();
//    QString strArg = "ping " + disSocket->peerAddress().toString() + " -n 1 -w " + QString::number(10000) ;
//    cmd->start(strArg);
//    cmd->waitForStarted();
//    cmd->waitForFinished();
//    qDebug() << QString::fromLocal8Bit(cmd->readAll());
//    QString retStr = cmd->readAll();
//    if (retStr.toStdString().find(aa) != -1)
//    {
//        qDebug() << "is online!\n";
//    }

    Player * tPlayer = getPlayerFromSocket( disSocket );
    if ( this->_playerQueue.contains(tPlayer) )    //是否还没匹配就掉了
    {
        this->_playerQueue.removeOne(tPlayer);

    }
    else     //对局中掉线，直接整局游戏结束
    {
        PlayerPair tPair = this->_matchedList[disSocket];
        Player * ePlayer = getEnemyFromSocket(disSocket);
        QTcpSocket * enemySocket = &(ePlayer->getSocket());
        //发送给对方，己方已经断开，游戏结束
        QDataStream out(enemySocket);
        out.setVersion(QDataStream::Qt_5_9);
        out<<8;
        enemySocket->flush();

        this->_playerQueue.enqueue(ePlayer);        //将对手放回就绪匹配队列
        this->_matchedList.remove(&(tPair.first->getSocket()));
        this->_matchedList.remove(&(tPair.second->getSocket()));
    }
    qDebug()<<"queue new size:"<<this->_playerQueue.size();
    disSocket->deleteLater();
}

void Widget::doRequest()
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

        break;
    default:
        break;
    }
}

void Widget::doMatch()
{
    Player* p1 = this->_playerQueue.dequeue();
    Player* p2 = this->_playerQueue.dequeue();
    PlayerPair pair(p1, p2);
    this->_matchedList.insert(&(p1->getSocket()), pair);
    this->_matchedList.insert(&(p2->getSocket()), pair);
    qDebug()<<"match succeed\n"<<"have "<<this->_matchedList.size()/2<<" match pair";
    //给双方发送匹配成功
    sendMessage(p1, 0);
    sendMessage(p1, p2->getPlayerName());
    p1->getSocket().flush();
    sendMessage(p2, 0);
    sendMessage(p2, p1->getPlayerName());
    p2->getSocket().flush();
}

void Widget::showError(QAbstractSocket::SocketError e)
{
    qDebug()<< e;
}

void Widget::sendMessage(Player* const player, int message){
    QTcpSocket* send = static_cast<QTcpSocket*>(&player->getSocket());
    QDataStream out(send);
    out.setVersion(QDataStream::Qt_5_9);
    out << message;
    qDebug()<<player->getPlayerName() + " out " + message + " succeed";
}

void Widget::sendMessage(Player* const player, QString message){
    QTcpSocket* send = static_cast<QTcpSocket*>(&player->getSocket());
    QDataStream out(send);
    out.setVersion(QDataStream::Qt_5_9);
    out << message;
    qDebug()<<player->getPlayerName() + " out " + message + " succeed";
}
