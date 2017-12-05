#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <utility>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    Ui::Widget getUi();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::Widget *ui;
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
