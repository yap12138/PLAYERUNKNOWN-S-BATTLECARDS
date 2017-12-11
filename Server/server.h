#ifndef SERVER_H
#define SERVER_H
#include <utility>
#include <QObject>
#include "player.h"

typedef std::pair<Player *, Player *> PlayerPair;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(Player* p1, Player* p2, QObject *parent = nullptr);
    ~Server();
    inline const PlayerPair & getPlayers() const { return this->_gamePair; }

private:
    static int _serialID;
    int _serverID;
    PlayerPair _gamePair;
    int _nextTurn;

    void bindServer(QTcpSocket &);
    Player* getPlayerFromSocket(QTcpSocket const *, int) const;    //mode=0找自己，mode=1找对手
    void sendMessage(Player* const player, int message);
    void sendMessage(Player * const player, const QString & message);
    void sendMessage(Player* const player, Card const * card);


    void enemySendCard(QTcpSocket const *, int, int, Card*);
    void dealSendMagic(Player *p1, Card *srcCard, int descID);

    void monsterAttack(Card *source, Card *target, Player* sourcePlayer, Player* targetPlayer);
    void monsterAttack(Card *source, Player *target, Player* sourcePlayer, Player* targetPlayer);
    void addArms(Card * sourceArms, Card *targetMonster, Player* sourcePlayer, Player* targetPlayer);

signals:
    void resetPlayer(Player*, Server*);
    void gameOver(Server*);

private slots:
    void doDisconnect();
    void doRequest();
    void doError(QAbstractSocket::SocketError);

    void doGameStart(); //开局
    void doTurnStart(); //回合开始

};

#endif // SERVER_H
