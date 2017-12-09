#include "cardwidget.h"
#include "ui_cardwidget.h"
#include <QBitmap>

CardWidget::CardWidget(QWidget *parent, int mode) :
    QWidget(parent),
    ui(new Ui::CardWidget)
{
    ui->setupUi(this);
    this->setGeometry(0,0,150*mode,200*mode);

    this->_card_bg = new QLabel(this);
    this->_consume_bg = new QLabel(this);
    this->_attack_bg = new QLabel(this);

    this->_card = new QLabel(this);
    this->_consume = new QLabel(this);
    this->_attack = new QLabel(this);

    setUpUi(mode);
}

CardWidget::~CardWidget()
{
    delete ui;
}

void CardWidget::setImage(const QPixmap &img)
{
    this->_card->setPixmap(img);
    //this->_card->setMask(img.mask());
}

void CardWidget::setUpUi(int mode)
{
    this->_card_bg->setGeometry(QRect(0, 0, 150*mode, 200*mode));
    this->_card_bg->setPixmap(QPixmap(":/cardWidget/resoure/img/card_bg.png"));
    this->_card_bg->setScaledContents(true);

    this->_consume_bg->setGeometry(QRect(60*mode,0,30*mode,30*mode));
    this->_consume_bg->setPixmap(QPixmap(":/cardWidget/resoure/img/consume.png"));
    this->_consume_bg->setScaledContents(true);

    this->_attack_bg->setGeometry(QRect(60*mode, 140*mode, 30*mode, 40*mode));
    this->_attack_bg->setPixmap(QPixmap(":/cardWidget/resoure/img/attack.png"));
    this->_attack_bg->setScaledContents(true);

    this->_card->setGeometry(QRect(30*mode, 20*mode, 90*mode, 130*mode));
    this->_card->setScaledContents(true);
    this->_card->lower();
    this->_card->setStyleSheet(QStringLiteral("background-color: rgb(79, 47, 22);"));

    QFont font;
    font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
    font.setPointSize(14*mode);
    font.setBold(true);
    font.setWeight(75);

    this->_consume->setGeometry(QRect(55*mode, 0, 40*mode, 30*mode));
    this->_consume->setFont(font);
    this->_consume->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    this->_consume->setStyleSheet(QStringLiteral("color: rgb(0, 0, 255);"));

    this->_attack->setGeometry(QRect(50*mode, 140*mode, 50*mode, 40*mode));
    this->_attack->setFont(font);
    this->_attack->setStyleSheet(QStringLiteral("color: rgb(255, 0, 4);"));
    this->_attack->setAlignment(Qt::AlignCenter);
}

void CardWidget::mousePressEvent(QMouseEvent *event)
{
    emit SMyPointer(this);
}
