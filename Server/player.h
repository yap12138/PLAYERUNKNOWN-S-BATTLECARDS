#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>
#include <QTcpSocket>
#include <QMap>
#include <QQueue>
#include <QStandardItem>
#include "carddeck.h"

class Player : public QObject
{
    Q_OBJECT
public:
    QQueue<int> _consumeForTurn;    //每回合得费队列

    explicit Player(QTcpSocket *socket, QObject *parent = nullptr);
    ~Player(){
        if (!this->_totalCardDeck.isEmpty())
        {
            foreach (auto var, this->_totalCardDeck) {
                this->_totalCardDeck.remove(var->getId());
                delete var;
            }
        }
    }
    inline QTcpSocket & getSocket() const { return this->_clientSocket; }
    inline const QString & getPlayerName() const { return this->_playerName; }
    inline void setPlayerName(QString & name) { this->_playerName = name; }

    inline Card * getCard(int cardId){ return this->_totalCardDeck[cardId]; }
    inline bool isMyCard(int cardId){ return this->_totalCardDeck.contains(cardId); }
    inline int getHP() const { return this->_HP; }
    inline void setHP(int HP) { this->_HP = HP; }
    inline int getConsume() const { return this->_consume; }
    inline void setConsume(int consume) { this->_consume = consume; }

    void initTotalCard(QStandardItemModel *model);
    int getNextConsume();
    Card const * getCardFromDeck();

private:
    QTcpSocket & _clientSocket; //和客户端的连接
    QString _playerName;        //玩家名字
    int _HP;
    int _consume;
    QMap<int, Card *> _totalCardDeck;   //玩家卡库
    QList<Card *> _restCard;

    void initConsumQueue(); //初始化每回合得费队列
signals:


public slots:

};

#endif // PLAYER_H
