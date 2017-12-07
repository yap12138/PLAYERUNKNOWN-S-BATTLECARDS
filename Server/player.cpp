#include "player.h"

Player::Player(QTcpSocket * socket, QObject *parent)
    : QObject(parent), _clientSocket(*socket)
{

}
