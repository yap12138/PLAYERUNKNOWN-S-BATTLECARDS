#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SERVER_H