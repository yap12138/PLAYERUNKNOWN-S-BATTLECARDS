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

    inline const PlayerPair & getPlayers() const { return this->_gamePair; }

private:
    static int _serialID;
    int _serverID;
    PlayerPair _gamePair;

    void bindServer(QTcpSocket &);
    Player* getPlayerFromSocket(QTcpSocket const *, int) const;    //mode=0找自己，mode=1找对手
    void sendMessage(Player* const player, int message);
    void sendMessage(Player * const player, QString message);

signals:
    void resetPlayer(Player*, Server*);

public slots:
    void doDisconnect();
    void doRequest();
    void doError(QAbstractSocket::SocketError);
};

#endif // SERVER_H