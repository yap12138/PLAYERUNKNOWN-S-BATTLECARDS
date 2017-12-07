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
    QQueue<Player *> _playerQueue;
    QHash<QTcpSocket *, PlayerPair> _matchedList;

private:
    void initServer();
    void initConnect();
    QHostAddress getHostConnectedIP() const;
    Player* getPlayerFromSocket(QTcpSocket const *) const;
    Player* getEnemyFromSocket(QTcpSocket const *) const;

    void getClientInfo(QTcpSocket* const socket, QDataStream & stream);

    void sendMessage( Player* const player, int message);
    void sendMessage(Player * const player, QString message);

signals:
    void canMatch();

private slots:
    void acceptConnection();
    void onDisConnect();
    void doRequest();
    void doMatch();
    void showError(QAbstractSocket::SocketError);
};

#endif // WIDGET_H
