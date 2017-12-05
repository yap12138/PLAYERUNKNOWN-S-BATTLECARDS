#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initClient();
}

Widget::~Widget()
{
    delete ui;
}

Ui::Widget Widget::getUi()
{
    return *ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->pushButton)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            _client = new QTcpSocket(this);
            _client->connectToHost(QHostAddress(ui->textEdit->toPlainText()),5000);
            QDataStream s(_client);
            s<<0;
            s<<QStringLiteral("asd");
            //sendMessage();
            return true;
        }
    }
    return false;
}

void Widget::initClient()
{
    ui->pushButton->installEventFilter(this);
    //client->connectToHost(QHostAddress());
}

QString Widget::getKeyFromSocket(const QTcpSocket *) const
{

}

void Widget::getClientInfo(QTcpSocket * const socket, QDataStream &stream)
{

}

void Widget::acceptConnection()
{

}

void Widget::onDisConnect()
{

}

void Widget::onReadyRead()
{

}

void Widget::sendMessage()
{
    QDataStream s(_client);
    s<<0;
    s<<QStringLiteral("zxc");
}
