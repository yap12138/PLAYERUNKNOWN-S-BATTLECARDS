#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initServer();
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
    _localHost = QHostAddress(QHostAddress::LocalHost);
    _server->listen(_localHost, 5000);
    connect(_server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    this->ui->lab_server->setText(_localHost.toString());
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
    foreach (auto var, this->_playerList) {
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

void Widget::getClientInfo(QTcpSocket * const socket, QDataStream &stream)
{
    QString name;
    stream>>name;
    qDebug()<<name;
    Player * var = getPlayerFromSocket(socket);

    var->setPlayerName(name);

    /*if (key.first == 1) {
        this->ui->lab_client1_name->setText(name);
    }
    else
    {
        this->ui->lab_client2_name->setText(name);
    }*/
}

void Widget::acceptConnection()
{
    QTcpSocket* clientConnection = _server->nextPendingConnection();
    qDebug()<<"new connect"<<clientConnection->localAddress().toString();
    connect(clientConnection, SIGNAL(disconnected()), this, SLOT(onDisConnect()));
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    //如果有可以匹配的玩家，则与之匹配，开始游戏
    if ( !this->_playerList.isEmpty() )
    {
        Player* p1 = this->_playerList.dequeue();
        Player* p2 = new Player(clientConnection, this);
        PlayerPair pair(p1, p2);
        this->_matchedList.insert(&(p1->getSocket()), pair);
        this->_matchedList.insert(clientConnection, pair);
        qDebug()<<"match succeed";
        //TODO 给双方发送匹配成功
    }
    else
    {
        //暂时无玩家匹配，进入队列等待
        Player* player = new Player(clientConnection, this);
        this->_playerList.enqueue(player);
    }

    /*if (this->_clientList.size()==0)
    {
        cKey key(1, keyString);
        this->_clientList.insert(key, clientConnection);
        this->ui->lab_client1->setText(keyString);
    }
    else if (this->_clientList.size()==1)
    {
        cKey key(2, keyString);
        this->_clientList.insert(key, clientConnection);
        this->ui->lab_client2->setText(keyString);
        //_server->close();

    }*/
    qDebug()<<"list new size:"<<this->_playerList.size();

}

void Widget::onDisConnect()
{
    int rec = this->_playerList.size();
    QTcpSocket* disSocket = qobject_cast<QTcpSocket*> (sender());
    Player * tPlayer = getPlayerFromSocket( disSocket );
    if ( this->_playerList.contains(tPlayer) )    //是否还没匹配就掉了
    {
        this->_playerList.removeOne(tPlayer);
    }
    qDebug()<<"list new size:"<<this->_playerList.size();
    disSocket->deleteLater();
    if (rec==1)
    {
        this->ui->lab_client1->setText(QStringLiteral(" "));
        this->ui->lab_client1_name->setText(QStringLiteral(" "));
    }
    else if (rec==2)
    {
        this->ui->lab_client2->setText(QStringLiteral(" "));
        this->ui->lab_client2_name->setText(QStringLiteral(" "));
        //_server->listen(_localHost, 5000);
    }
}

void Widget::onReadyRead()
{
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
