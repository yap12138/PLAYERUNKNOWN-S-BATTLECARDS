#ifndef WIDGET_H
#define WIDGET_H
//#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QtNetwork>
#include <QHash>
#include <utility>

using std::pair;

typedef pair<int, QString> cKey;    //存放客户端socket的Hash容器的键

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
    QHash<cKey, QTcpSocket *> _clientList;

private:
    void initServer();
    QHostAddress getHostConnectedIP() const;
    QString getKeyFromSocket(const QTcpSocket*) const;

    void getClientInfo(QTcpSocket* const socket, QDataStream & stream);

private slots:
    void acceptConnection();
    void onDisConnect();
    void onReadyRead();
};

#endif // WIDGET_H
