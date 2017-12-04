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
}

void Widget::initServer()
{
    _server = new QTcpServer(this);
    QHostAddress localHost = getHostConnectedIP();
    _server->listen(localHost, 5000);
    this->ui->lab_server->setText(localHost.toString());
    connect(_server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

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
            if (interface.humanReadableName().contains("VirtualBox") || interface.humanReadableName().contains("本地连接") )
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

QString Widget::getKeyFromSocket(const QTcpSocket * socket) const
{
    return socket->peerAddress().toString()+" : "+QString::number(socket->peerPort());
}

void Widget::getClientInfo(QTcpSocket * const socket, QDataStream &stream)
{
    QString name;
    stream>>name;
    qDebug()<<name;
    cKey key = this->_clientList.key(socket);
    if (key.first == 1) {
        this->ui->lab_client1_name->setText(name);
    }
    else
    {
        this->ui->lab_client2_name->setText(name);
    }
}

void Widget::acceptConnection()
{
    QTcpSocket* clientConnection = _server->nextPendingConnection();
    qDebug()<<"new connect"<<clientConnection->localAddress().toString();
    connect(clientConnection, SIGNAL(disconnected()), this, SLOT(onDisConnect()));
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    QString keyString = getKeyFromSocket(clientConnection);

    if (this->_clientList.size()==0)
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
        //_server->pauseAccepting();
        _server->close();
    }
    qDebug()<<"list new size:"<<this->_clientList.size();

}

void Widget::onDisConnect()
{
    int rec = this->_clientList.size();
    QTcpSocket* disSocket = qobject_cast<QTcpSocket*> (sender());
    cKey key = this->_clientList.key(disSocket);
    this->_clientList.remove(key);
    qDebug()<<"list new size:"<<this->_clientList.size();
    disSocket->deleteLater();
    if (rec==1)
    {
        this->ui->lab_client1->setText(QStringLiteral(" "));
        this->ui->lab_client1_name->setText(QStringLiteral(" "));
    }
    else if (rec==2)
    {
        //_server->resumeAccepting();
        this->ui->lab_client2->setText(QStringLiteral(" "));
        this->ui->lab_client2_name->setText(QStringLiteral(" "));
        _server->listen(getHostConnectedIP(), 5000);
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
