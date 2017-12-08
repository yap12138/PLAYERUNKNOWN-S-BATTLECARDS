#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>
#include <QTcpSocket>
#include <QList>
#include "carddeck.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QTcpSocket *socket, QObject *parent = nullptr);

    inline QTcpSocket & getSocket() const { return this->_clientSocket; }
    inline const QString & getPlayerName() const { return this->_playerName; }
    inline void setPlayerName(QString & name) { this->_playerName = name; }

private:
    QTcpSocket & _clientSocket; //和客户端的连接
    QString _playerName;        //玩家名字
    QList<Card *> _totalCardDeck;

signals:

public slots:
};

#endif // PLAYER_H