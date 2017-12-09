#include "connectwidget.h"
#include "ui_connectwidget.h"
#include "cardwidget.h"

ConnectWidget::ConnectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectWidget)
{
    ui->setupUi(this);
    initClient();
    CardWidget* cardWidget = new CardWidget(this, 2);
    cardWidget->setGeometry(0,0,cardWidget->width(),cardWidget->height());
}

ConnectWidget::~ConnectWidget()
{
    delete ui;
}

Ui::ConnectWidget ConnectWidget::getUi()
{
    return *ui;
}

bool ConnectWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->pushButton)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
//            _client = new QTcpSocket(this);
//            _client->connectToHost(QHostAddress(ui->textEdit->toPlainText()),5000);
//            QDataStream s(_client);
//            s<<0;
//            s<<QStringLiteral("asd");
//            sendMessage();
            this->hide();
            return true;
        }
    }
    return false;
}

void ConnectWidget::initClient()
{
    ui->pushButton->installEventFilter(this);
    //client->connectToHost(QHostAddress());
}

QString ConnectWidget::getKeyFromSocket(const QTcpSocket *) const
{

}

void ConnectWidget::getClientInfo(QTcpSocket * const socket, QDataStream &stream)
{

}

void ConnectWidget::acceptConnection()
{

}

void ConnectWidget::onDisConnect()
{

}

void ConnectWidget::onReadyRead()
{

}

void ConnectWidget::sendMessage()
{
    QDataStream s(_client);
    s<<0;
    s<<QStringLiteral("zxc");
}
