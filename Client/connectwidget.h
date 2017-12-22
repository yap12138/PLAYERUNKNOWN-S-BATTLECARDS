#ifndef CONNECTWIDGET_H
#define CONNECTWIDGET_H

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
    explicit ConnectWidget(QTcpSocket *client,QWidget *parent = 0);
    ~ConnectWidget();

    QString getName();

    //@yap
    void resetWidget();

private:
    Ui::ConnectWidget *ui;
    QTcpSocket *_client;

    //@yap
private slots:
    void tryToConnect();
};

#endif // WIDGET_H
