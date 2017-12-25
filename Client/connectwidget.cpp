#include "connectwidget.h"
#include "ui_connectwidget.h"
#include <QString>
#include <QTcpSocket>
#include <QApplication>
#include <QMessageBox>

ConnectWidget::ConnectWidget(QTcpSocket *client, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectWidget)
{
    _client =  client;
    ui->setupUi(this);

    //@yap
    ui->_ip_input->setText(QStringLiteral("127.0.0.1"));

    connect(ui->_conn_btn, SIGNAL(clicked(bool)), this, SLOT(tryToConnect()));
}

ConnectWidget::~ConnectWidget()
{
    delete ui;
}

QString ConnectWidget::getName()
{
    return ui->_username_input->toPlainText();
}

//@yap
void ConnectWidget::resetWidget()
{
    QApplication::restoreOverrideCursor();
    this->hide();
}

//@yap
void ConnectWidget::tryToConnect()
{
    if (_client->state() == QAbstractSocket::ConnectingState
            || _client->state() == QAbstractSocket::ConnectedState)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("正在连接中/已连接"));
        return;
    }

    QHostAddress *Address = new QHostAddress(ui->_ip_input->toPlainText());
    _client->connectToHost(*Address,5000);

    if ( !_client->waitForConnected(10000) )
    {
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QDataStream s(_client);
    s<<0;
    s<<ui->_username_input->toPlainText();
    _client->flush();
}



