#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H
//#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QtNetwork>
#include <QHash>
#include <QQueue>
#include <QStandardItem>
#include "server.h"


namespace Ui {
class Widget;
}

class ServerBootstrap : public QWidget
{
    Q_OBJECT
public:
    explicit ServerBootstrap(QWidget *parent = 0);
    ~ServerBootstrap();

private:
    Ui::Widget *ui;
    QStandardItemModel *_model;

    QTcpServer *_server;
    QHostAddress _localHost;
    QQueue<Player *> _playerQueue;
    QHash<Player *, Server *> _matchedList; //每一居存放两个键值对 eg：<p1,server>, <p2,server>

private:
    void initServer();
    void initConnect();
    QHostAddress getHostConnectedIP() const;
    Player* getPlayerFromSocket(QTcpSocket const *) const;

    void getClientInfo(QTcpSocket* const socket, QDataStream & stream);
signals:
    void canMatch();

private slots:
    void acceptConnection();
    void onDisConnect();
    void doRequest();
    void doMatch();
    void showError(QAbstractSocket::SocketError);
    void doReset(Player*, Server*);
    void doGameover(Server*);
};

#endif // BOOTSTRAP_H
