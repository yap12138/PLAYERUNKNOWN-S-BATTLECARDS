#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>
#include <QTcpSocket>
#include <QHash>
#include <QQueue>
#include "carddeck.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QTcpSocket *socket, QObject *parent = nullptr);

    inline QTcpSocket & getSocket() const { return this->_clientSocket; }
    inline const QString & getPlayerName() const { return this->_playerName; }
    inline void setPlayerName(QString & name) { this->_playerName = name; }

    void initTotalCard();
    int getNextConsume();
    Card const * getCardFromDeck();
private:
    QTcpSocket & _clientSocket; //和客户端的连接
    QString _playerName;        //玩家名字
    QHash<int, Card *> _totalCardDeck;   //玩家卡组

    QQueue<int> _consumeForTurn;    //每回合得费队列

    void initConsumQueue(); //初始化每回合得费队列
signals:
    void useCard();

public slots:

};

#endif // PLAYER_H