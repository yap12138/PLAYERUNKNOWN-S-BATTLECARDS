#include "cardwidget.h"
#include "ui_cardwidget.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

CardWidget::CardWidget(QWidget *parent, int mode) :
    QWidget(parent),
    ui(new Ui::CardWidget)
{
    ui->setupUi(this);
    this->setGeometry(0,0,150*mode,200*mode);

    this->_card_bg = new QLabel(this);
    this->_consume_bg = new QLabel(this);
    this->_attack_bg = new QLabel(this);
    this->_weapon_bg = new QLabel(this);

    this->_card = new QLabel(this);
    this->_consume = new QLabel(this);
    this->_attack = new QLabel(this);
    this->_weapon = new QLabel(this);

    //@yap
    this->_weapon_bg->hide();

    this->setCursor(Qt::PointingHandCursor);

    setUpUi(mode);

    initAlpha();
    // 设置窗口部件背景半透明
//    QPalette myPalette;
//    QColor myColor(255,255,255);
//    myColor.setAlphaF(0.7);
//    myPalette.setBrush(backgroundRole(),myColor);
//    this->setPalette(myPalette);
//    this->setAutoFillBackground(true);
}

CardWidget::~CardWidget()
{
    delete ui;
    if (_realCard != nullptr)
        delete _realCard;
}

void CardWidget::setImage(const QPixmap &var_img)
{
    if (&var_img == 0)
    {
        qDebug()<<"null pixmap";
        return;
    }
    QPixmap img = var_img.scaled(_card->size(), Qt::KeepAspectRatio);

    this->_card->setPixmap(img);
    this->_s_card = img;
    //this->_card->setMask(img.mask());

    _a_card = QPixmap(_s_card.size());
    _a_card.fill(Qt::transparent);
    QPainter p1(&_a_card);
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0, 0, _s_card);
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(_a_card.rect(), QColor(255, 255, 255, 140));
    p1.end();
}

/**
 * @brief CardWidget::show
 * @param mode  0表示手上显示，1表示场上显示
 */
void CardWidget::show(int mode)
{
    if (mode == 0)
    {
        QWidget::show();
        return;
    }
    else
    {
        this->_consume_bg->hide();
        this->_consume->hide();
        QWidget::show();
        return;
    }
}

/**
 * @brief CardWidget::setOpacity
 * @param level 0 表示半透明，1不透明
 */
void CardWidget::setOpacity(int level)
{
//    this->_card_bg->setWindowOpacity(level);
//    this->_consume_bg->setWindowOpacity(level);
//    this->_attack_bg->setWindowOpacity(level);
//    this->_weapon_bg->setWindowOpacity(level);

//    this->_card->setWindowOpacity(level);
//    this->_consume->setWindowOpacity(level);
//    this->_attack->setWindowOpacity(level);
//    this->_weapon->setWindowOpacity(level);

    if ( level == 0 )
    {
        this->_card_bg->setPixmap(_a_card_bg);
        this->_consume_bg->setPixmap(_a_consume_bg);
        this->_attack_bg->setPixmap(_a_attack_bg);
        this->_weapon_bg->setPixmap(_a_weapon_bg);
        this->_card->setPixmap(_a_card);

        this->_attack->setStyleSheet(QStringLiteral("color: rgba(255, 0, 4, 140);"));
        this->_consume->setStyleSheet(QStringLiteral("color: rgba(0, 0, 255, 140);"));
        this->_weapon->setStyleSheet(QStringLiteral("color: rgba(34, 255, 0, 140);"));
    }
    else
    {
        this->_card_bg->setPixmap(_s_card_bg);
        this->_consume_bg->setPixmap(_s_consume_bg);
        this->_attack_bg->setPixmap(_s_attack_bg);
        this->_weapon_bg->setPixmap(_s_weapon_bg);
        this->_card->setPixmap(_s_card);

        this->_attack->setStyleSheet(QStringLiteral("color: rgb(255, 0, 4);"));
        this->_consume->setStyleSheet(QStringLiteral("color: rgb(0, 0, 255);"));
        this->_weapon->setStyleSheet(QStringLiteral("color: rgb(34, 255, 0);"));
    }

}

void CardWidget::initAlpha()
{
    _a_attack_bg.fill(Qt::transparent);
    QPainter p1(&_a_attack_bg);
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0, 0, _s_attack_bg);
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(_a_attack_bg.rect(), QColor(255, 255, 255, 140));
    p1.end();

    _a_card_bg.fill(Qt::transparent);
    QPainter p2(&_a_card_bg);
    p2.setCompositionMode(QPainter::CompositionMode_Source);
    p2.drawPixmap(0, 0, _s_card_bg);
    p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p2.fillRect(_a_card_bg.rect(), QColor(255, 255, 255, 140));
    p2.end();

    _a_consume_bg.fill(Qt::transparent);
    QPainter p3(&_a_consume_bg);
    p3.setCompositionMode(QPainter::CompositionMode_Source);
    p3.drawPixmap(0, 0, _s_consume_bg);
    p3.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p3.fillRect(_a_consume_bg.rect(), QColor(255, 255, 255, 140));
    p3.end();

    _a_weapon_bg.fill(Qt::transparent);
    QPainter p4(&_a_weapon_bg);
    p4.setCompositionMode(QPainter::CompositionMode_Source);
    p4.drawPixmap(0, 0, _s_weapon_bg);
    p4.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p4.fillRect(_a_weapon_bg.rect(), QColor(255, 255, 255, 140));
    p4.end();
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

    this->_weapon_bg->setGeometry(QRect(117*mode,0,30*mode,30*mode));
    this->_weapon_bg->setPixmap(QPixmap(":/cardWidget/resoure/img/attack.png"));
    this->_weapon_bg->setScaledContents(true);

    this->_card->setGeometry(QRect(30*mode, 20*mode, 90*mode, 130*mode));
    //this->_card->setScaledContents(true);
    this->_card->setAlignment(Qt::AlignCenter);
    this->_card->lower();
    this->_card->setStyleSheet(QStringLiteral("background-color: rgba(79, 47, 22, 140);"));

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

    this->_weapon->setGeometry(QRect(113*mode,0,40*mode,30*mode));
    this->_weapon->setFont(font);
    this->_weapon->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    this->_weapon->setStyleSheet(QStringLiteral("color: rgb(34, 255, 0);"));
}

void CardWidget::mousePressEvent(QMouseEvent *event)
{
    emit SMyPointer(this);
    event->accept();
}
