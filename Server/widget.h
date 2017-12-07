#ifndef WIDGET_H
#define WIDGET_H
//#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QtNetwork>
#include <QHash>
#include <QQueue>
#include <utility>
#include "player.h"

//using std::pair;

typedef std::pair<Player *, Player *> PlayerPair;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpServer *_server;
    QHostAddress _localHost;
    QQueue<Player *> _playerList;
    QHash<QTcpSocket *, PlayerPair> _matchedList;

private:
    void initServer();
    QHostAddress getHostConnectedIP() const;
    Player* getPlayerFromSocket(QTcpSocket const *) const;

    void getClientInfo(QTcpSocket* const socket, QDataStream & stream);

private slots:
    void acceptConnection();
    void onDisConnect();
    void onReadyRead();
};

#endif // WIDGET_H
