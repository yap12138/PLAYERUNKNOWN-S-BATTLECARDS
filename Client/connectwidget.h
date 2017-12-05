#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <utility>
namespace Ui {
class ConnectWidget;
}

class ConnectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectWidget(QWidget *parent = 0);
    ~ConnectWidget();
    Ui::ConnectWidget getUi();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::ConnectWidget *ui;
    QTcpSocket *_client;
private:
    void initClient();
    QString getKeyFromSocket(const QTcpSocket*) const;
    void getClientInfo(QTcpSocket* const socket, QDataStream & stream);

private slots:
    void acceptConnection();//接受连接，保存
    void onDisConnect();//断开连接
    void onReadyRead();//准备读取
    void sendMessage();//发送信息
};

#endif // WIDGET_H
